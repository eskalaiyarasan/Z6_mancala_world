import os
import json
import sys
from datetime import datetime
def generate_cpp_from_schemas(schema_dir, output_dir):
    os.makedirs(output_dir, exist_ok=True)

    schema_classes_map = {}
    class_to_file_map = {}

    # First pass: parse schemas and map classes to their source filenames
    for filename in sorted(os.listdir(schema_dir)):
        if not filename.endswith(".json"):
            continue

        filepath = os.path.join(schema_dir, filename)
        with open(filepath, "r") as f:
            try:
                schema_data = json.load(f)
            except Exception as e:
                print(f"exception loading {filename}: {e}")
                continue

        defs = schema_data.get("$defs", {})
        file_classes = []
        for raw_class_name, class_def in defs.items():
            if class_def.get("type") != "object":
                continue
            class_name = f"Z6{raw_class_name}Json"
            file_classes.append((raw_class_name, class_def))
            schema_base_name = os.path.splitext(filename)[0]
            class_to_file_map[class_name] = schema_base_name.lower()

        schema_classes_map[filename] = file_classes

    def get_cpp_type(prop_schema, prop_name):
        if "$ref" in prop_schema:
            ref_name = prop_schema["$ref"].split("/")[-1]
            return f"Z6{ref_name}Json"

        t = prop_schema.get("type")
        if t == "boolean":
            return "bool"
        elif t == "integer":
            return "int"
        elif t == "string":
            return "std::string"
        elif t == "object":
            return "Z6json"
        elif t == "array":
            items = prop_schema.get("items", {})
            if "$ref" in items:
                item_type = f"Z6{items['$ref'].split('/')[-1]}Json"
            else:
                item_type = get_cpp_type(items, "item")
            return f"std::vector<{item_type}>"
        return "std::string"

    # Second pass: Generate separate .h and .cpp files with declarations in .h and definitions in .cpp
    for filename, file_classes in schema_classes_map.items():
        schema_base_name = os.path.splitext(filename)[0]
        header_declarations = []
        cpp_definitions = []
        factory_cases = []
        included_headers = set()

        for raw_class_name, class_def in file_classes:
            class_name = f"Z6{raw_class_name}Json"
            factory_cases.append(f'        if (type == "{raw_class_name}" || type == "{class_name}") return std::make_unique<{class_name}>();')

            properties = class_def.get("properties", {})
            required_props = class_def.get("required", [])

            fields_decl = []
            constructor_inits = []
            copy_constructor_inits = []
            getters_decl = []
            setters_decl = []

            getters_cpp = []
            setters_cpp = []
            from_json_lines = []
            to_json_lines = []
            validate_lines = []

            # Basic JSON syntax check in validation function
            validate_lines.append("    try {")
            validate_lines.append("        auto j = json::parse(json_str);")
            validate_lines.append("        if (!j.is_object()) return false;")

            for req in required_props:
                validate_lines.append(f'        if (!j.contains("{req}")) return false;')

            for prop_name, prop_schema in properties.items():
                cpp_type = get_cpp_type(prop_schema, prop_name)

                # Check for dependencies to include headers
                clean_type = cpp_type.replace("std::vector<", "").replace(">", "")
                if clean_type in class_to_file_map:
                    dep_file = class_to_file_map[clean_type]
                    if dep_file != schema_base_name.lower():
                        included_headers.add(dep_file)

                validate_lines.append(f'        if (j.contains("{prop_name}")) {{')
                if cpp_type == "bool":
                    validate_lines.append(f'            if (!j["{prop_name}"].is_boolean()) return false;')
                elif cpp_type == "int":
                    validate_lines.append(f'            if (!j["{prop_name}"].is_number_integer()) return false;')
                elif cpp_type == "std::string":
                    validate_lines.append(f'            if (!j["{prop_name}"].is_string()) return false;')
                elif cpp_type.startswith("std::vector<"):
                    validate_lines.append(f'            if (!j["{prop_name}"].is_array()) return false;')
                else:
                    validate_lines.append(f'            if (!j["{prop_name}"].is_object()) return false;')
                validate_lines.append(f'        }}')

            validate_lines.append("        return true;")
            validate_lines.append("    } catch (...) {")
            validate_lines.append("        return false;")
            validate_lines.append("    }")

            for prop_name, prop_schema in properties.items():
                cpp_type = get_cpp_type(prop_schema, prop_name)
                field_name = f"{prop_name[0].lower()}{prop_name[1:]}_"
                valid_name = f"{field_name}valid_"

                fields_decl.append(f"    {cpp_type} {field_name};")
                fields_decl.append(f"    bool {valid_name};")

                default_val = prop_schema.get("default")
                if default_val is not None:
                    if cpp_type == "std::string":
                        constructor_inits.append(f"{field_name}(\"{default_val}\")")
                    elif cpp_type == "bool":
                        constructor_inits.append(f"{field_name}({str(default_val).lower()})")
                    else:
                        constructor_inits.append(f"{field_name}({default_val})")
                else:
                    if cpp_type in ["int", "double", "float"]:
                        constructor_inits.append(f"{field_name}(0)")
                    elif cpp_type == "bool":
                        constructor_inits.append(f"{field_name}(false)")

                constructor_inits.append(f"{valid_name}(false)")
                copy_constructor_inits.append(f"{field_name}(other.{field_name})")
                copy_constructor_inits.append(f"{valid_name}(other.{valid_name})")

                if cpp_type.startswith("std::vector<"):
                    inner_type = cpp_type[12:-1]
                    getters_decl.append(f"    bool get{prop_name}(size_t index, {inner_type}& val) const;")
                    getters_cpp.append(f"""bool {class_name}::get{prop_name}(size_t index, {inner_type}& val) const {{
    if (!{valid_name} || index >= {field_name}.size()) return false;
    val = {field_name}[index];
    return true;
}}""")
                    getters_decl.append(f"    bool get{prop_name}Vector({cpp_type}& val) const;")
                    getters_cpp.append(f"""bool {class_name}::get{prop_name}Vector({cpp_type}& val) const {{
    if (!{valid_name}) return false;
    val = {field_name};
    return true;
}}""")
                else:
                    getters_decl.append(f"    bool get{prop_name}({cpp_type}& val) const;")
                    getters_cpp.append(f"""bool {class_name}::get{prop_name}({cpp_type}& val) const {{
    if (!{valid_name}) return false;
    val = {field_name};
    return true;
}}""")

                setters_decl.append(f"    void set{prop_name}(const {cpp_type}& val);")
                setters_cpp.append(f"""void {class_name}::set{prop_name}(const {cpp_type}& val) {{
    {field_name} = val;
    {valid_name} = true;
}}""")

                if cpp_type.startswith("std::vector<") and not ("std::vector<int>" in cpp_type or "std::vector<std::string>" in cpp_type):
                    from_json_lines.append(f"""        if (j.contains("{prop_name}")) {{
            {field_name}.clear();
            for (const auto& item : j["{prop_name}"]) {{
                {inner_type} obj;
                obj.from_json(item.dump());
                {field_name}.push_back(obj);
            }}
            {valid_name} = true;
        }}""")
                elif cpp_type in ["int", "bool", "std::string"]:
                    from_json_lines.append(f"""        if (j.contains("{prop_name}")) {{
            {field_name} = j["{prop_name}"];
            {valid_name} = true;
        }}""")
                elif "std::vector<" in cpp_type:
                    from_json_lines.append(f"""        if (j.contains("{prop_name}")) {{
            {field_name} = j["{prop_name}"].get<{cpp_type}>();
            {valid_name} = true;
        }}""")
                else:
                    from_json_lines.append(f"""        if (j.contains("{prop_name}")) {{
            {field_name}.from_json(j["{prop_name}"].dump());
            {valid_name} = true;
        }}""")

                if cpp_type.startswith("std::vector<") and not ("std::vector<int>" in cpp_type or "std::vector<std::string>" in cpp_type):
                    to_json_lines.append(f"""        if ({valid_name}) {{
            json arr = json::array();
            for (const auto& item : {field_name}) {{
                arr.push_back(json::parse(item.to_json()));
            }}
            j["{prop_name}"] = arr;
        }}""")
                elif not cpp_type.startswith("std::vector<") and cpp_type not in ["int", "bool", "std::string"]:
                    to_json_lines.append(f"""        if ({valid_name}) {{
            j["{prop_name}"] = json::parse({field_name}.to_json());
        }}""")
                else:
                    to_json_lines.append(f"""        if ({valid_name}) {{
            j["{prop_name}"] = {field_name};
        }}""")

            init_list = ", ".join(constructor_inits)
            if init_list:
                init_list = " : " + init_list

            copy_init_list = ", ".join(copy_constructor_inits)
            if copy_init_list:
                copy_init_list = " : " + copy_init_list

            assignment_copies = []
            for prop_name in properties.keys():
                field_name = f"{prop_name[0].lower()}{prop_name[1:]}_"
                valid_name = f"{field_name}valid_"
                assignment_copies.append(f"        {field_name} = other.{field_name};")
                assignment_copies.append(f"        {valid_name} = other.{valid_name};")

            # Class Declaration for Header File (.h)
            class_decl = f"""class {class_name} : public Z6json {{
private:
{'\n'.join(fields_decl)}

public:
    {class_name}(){init_list} {{}}
    {class_name}(const {class_name}& other){copy_init_list} {{}}
    {class_name}& operator=(const {class_name}& other);

    static bool validate(const std::string& json_str);

{'\n'.join(getters_decl)}

{'\n'.join(setters_decl)}

    void from_json(const std::string& json_str) override;
    std::string to_json() const override;
}};
"""
            header_declarations.append(class_decl)

            # Class Definitions for Implementation File (.cpp)
            assignment_def = f"""{class_name}& {class_name}::operator=(const {class_name}& other) {{
    if (this != &other) {{
{'\n'.join(assignment_copies)}
    }}
    return *this;
}}"""

            validate_def = f"""bool {class_name}::validate(const std::string& json_str) {{
{'\n'.join(validate_lines)}
}}"""

            from_json_def = f"""void {class_name}::from_json(const std::string& json_str) {{
    json j = json::parse(json_str);
{'\n'.join(from_json_lines)}
}}"""

            to_json_def = f"""std::string {class_name}::to_json() const {{
    json j;
{'\n'.join(to_json_lines)}
    return j.dump();
}}"""

            cpp_definitions.append(assignment_def)
            cpp_definitions.append(validate_def)
            cpp_definitions.append('\n\n'.join(getters_cpp))
            cpp_definitions.append('\n\n'.join(setters_cpp))
            cpp_definitions.append(from_json_def)
            cpp_definitions.append(to_json_def)

        # Build dependent headers text for .h or .cpp
        dep_includes_header = ""
        for inc in sorted(included_headers):
            if inc != schema_base_name.lower():
                dep_includes_header += f'#include "{inc.replace(".", "_")}.h"\n'

        header_content = f"""/**  autogenerated code ***
     * @file {schema_base_name.replace(".", "_")}.h
     * @author  --
     * @date {str(datetime.now())}
     *
     * @copyright Copyright (c) 2026. All rights reserved.
     * This code and all associated intellectual property completely
     * belong to the ZERASIX GAMING STUDIO LLP.
     * Unauthorized copying, distribution, or use is strictly prohibited.
     */
#ifndef {schema_base_name.upper().replace(".", "_")}_H
#define {schema_base_name.upper().replace(".", "_")}_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <z6_base_json.h>
{dep_includes_header}
using json = nlohmann::json;



{'\n'.join(header_declarations)}

#endif // {schema_base_name.upper().replace(".", "_")}_H
"""

        src_content = f"""/**  autogenerated code ***
     * @file {schema_base_name.replace(".", "_")}.cpp
     * @author  --
     * @date {str(datetime.now())}
     *
     * @copyright Copyright (c) 2026. All rights reserved.
     * This code and all associated intellectual property completely
     * belong to the ZERASIX GAMING STUDIO LLP.
     * Unauthorized copying, distribution, or use is strictly prohibited.
     */
#include "{schema_base_name.replace(".", "_")}.h"

{'\n\n'.join([d for d in cpp_definitions if d.strip()])}

"""

        hfile = os.path.join(output_dir, f"{schema_base_name.replace(".", "_")}.h")
        cppfile = os.path.join(output_dir, f"{schema_base_name.replace(".", "_")}.cpp")

        with open(hfile, "w") as fp:
            fp.write(header_content)
        with open(cppfile, "w") as fp:
            fp.write(src_content)

        print(f"Generated {hfile} and {cppfile}")

def generate_base_files(output_dir):
    os.makedirs(output_dir, exist_ok=True)
    data="""/**  autogenerated code ***
* @file z6_base_json.h
* @author  --
* @date {str(datetime.now())}
*
* @copyright Copyright (c) 2026. All rights reserved.
* This code and all associated intellectual property completely
* belong to the ZERASIX GAMING STUDIO LLP.
* Unauthorized copying, distribution, or use is strictly prohibited.
*/
#pragma once
class Z6json {
   public:
       virtual ~Z6json() = default;
       virtual void from_json(const std::string& json_str) = 0;
       virtual std::string to_json() const = 0;
   };
"""
    hfile = os.path.join(output_dir,"z6_base_json.h")
    with open(hfile, "w") as fp:
        fp.write(data)


if len(sys.argv) > 2:
    generate_base_files(sys.argv[2])
    generate_cpp_from_schemas(sys.argv[1], sys.argv[2])


"""class Z6jsonFactory_{schema_base_name.replace(".", "_")} {{
public:
    static std::unique_ptr<Z6json> createObject(const std::string& type) {{
{'\n'.join(factory_cases)}
        return nullptr;
    }}
}};"""
