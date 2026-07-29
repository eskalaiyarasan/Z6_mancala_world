import os
import json

def generate_cpp_from_schemas(schema_dir):
    classes_code = []
    factory_cases = []

    # Map json-schema types/formats to C++ types
    def get_cpp_type(prop_schema, prop_name):
        if "$ref" in prop_schema:
            ref_name = prop_schema["$ref"].split("/")[-1]
            return ref_name
        
        t = prop_schema.get("type")
        if t == "boolean":
            return "bool"
        elif t == "integer":
            return "int"
        elif t == "string":
            return "std::string"
        elif t == "array":
            items = prop_schema.get("items", {})
            if "$ref" in items:
                item_type = items["$ref"].split("/")[-1]
            else:
                item_type = get_cpp_type(items, "item")
            return f"std::vector<{item_type}>"
        return "std::string"

    # Read all schema files in directory
    for filename in sorted(os.listdir(schema_dir)):
        if not filename.endswith(".json"):
            continue
        
        filepath = os.path.join(schema_dir, filename)
        with open(filepath, "r") as f:
            try:
                schema_data = json.load(f)
            except Exception as e:
                continue

        defs = schema_data.get("$defs", {})
        for class_name, class_def in defs.items():
            if class_def.get("type") != "object":
                continue

            factory_cases.append(f'        if (type == "{class_name}") return std::make_unique<{class_name}>();')
            
            properties = class_def.get("properties", {})
            
            # Generate fields and validity flags
            fields_decl = []
            constructor_inits = []
            getters = []
            setters = []
            from_json_lines = []
            to_json_lines = []

            for prop_name, prop_schema in properties.items():
                cpp_type = get_cpp_type(prop_schema, prop_name)
                field_name = f"{prop_name[0].lower()}{prop_name[1:]}_"
                valid_name = f"{field_name}valid_"

                fields_decl.append(f"    {cpp_type} {field_name};")
                fields_decl.append(f"    bool {valid_name};")
                
                # Constructor initialization
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

                # Getter implementation with validation flag & array index check
                if cpp_type.startswith("std::vector<"):
                    inner_type = cpp_type[12:-1]
                    getters.append(f"""    bool get{prop_name}(size_t index, {inner_type}& val) const {{
        if (!{valid_name} || index >= {field_name}.size()) return false;
        val = {field_name}[index];
        return true;
    }}""")
                    getters.append(f"""    bool get{prop_name}Vector({cpp_type}& val) const {{
        if (!{valid_name}) return false;
        val = {field_name};
        return true;
    }}""")
                else:
                    getters.append(f"""    bool get{prop_name}({cpp_type}& val) const {{
        if (!{valid_name}) return false;
        val = {field_name};
        return true;
    }}""")

                # Setter implementation
                setters.append(f"""    void set{prop_name}(const {cpp_type}& val) {{
        {field_name} = val;
        {valid_name} = true;
    }}""")

                # from_json logic
                if cpp_type.startswith("std::vector<") and not ("std::vector<int>" in cpp_type or "std::vector<std::string>" in cpp_type):
                    # Complex object vector parsing
                    from_json_lines.append(f"""        if (j.contains("{prop_name}")) {{
            {field_name}.clear();
            for (const auto& item : j["{prop_name}"]) {{
                {inner_type} obj;
                obj.from_json(item.dump());
                {field_name}.push_back(obj);
            }}
            {valid_name} = true;
        }}""")
                elif cpp_type == "int" or cpp_type == "bool" or cpp_type == "std::string":
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
                    # Nested single object reference
                    from_json_lines.append(f"""        if (j.contains("{prop_name}")) {{
            {field_name}.from_json(j["{prop_name}"].dump());
            {valid_name} = true;
        }}""")

                # to_json logic
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

            # Construct class string
            init_list = ", ".join(constructor_inits)
            if init_list:
                init_list = " : " + init_list

            class_code = f"""class {class_name} : public Z6json {{
private:
{'\n'.join(fields_decl)}

public:
    {class_name}(){init_list} {{}}

{'\n\n'.join(getters)}

{'\n\n'.join(setters)}

    void from_json(const std::string& json_str) override {{
        json j = json::parse(json_str);
{'\n'.join(from_json_lines)}
    }}

    std::string to_json() const override {{
        json j;
{'\n'.join(to_json_lines)}
        return j.dump();
    }}
}};
"""
            classes_code.append(class_code)

    # Generate final complete C++ file header and content
    header_content = f"""#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Z6json {{
public:
    virtual ~Z6json() = default;
    virtual void from_json(const std::string& json_str) = 0;
    virtual std::string to_json() const = 0;
}};

{'\n'.join(classes_code)}

class Z6jsonFactory {{
public:
    static std::unique_ptr<Z6json> createObject(const std::string& type) {{
{'\n'.join(factory_cases)}
        return nullptr;
    }}
}};
"""
    return header_content

# Example usage:
# cpp_code = generate_cpp_from_schemas("./schemas")
# print(cpp_code)