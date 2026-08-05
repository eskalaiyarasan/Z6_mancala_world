import socket
import os
import sys
import json

SERVER_SOCKET_PATH = "/tmp/.z6_main.sock"

# 1. Define a clean Python dictionary as a baseline template
BASE_TEMPLATE = {
    "schema_version": 1,
    "Name": "INDIAN_CLASSIC_SURYA",
    "Level": 1,
    "Human": True,
    "Players": []
}

def get_user_inputs():
    print("\n--- Configure Your Test Payload ---")

    # Select Game Mode Name
    print("Available Game Modes:")
    print("  1: INDIAN_CLASSIC_SURYA")
    print("  2: INDIAN_CLASSIC_DHARMA")
    print("  3: INDIAN_KINGS_SURYA")
    print("  4: INDIAN_KINGS_DHARMA")
    mode_choice = input("Select Game Mode [1-4] (Default 1): ").strip()

    modes = {
        "1": "INDIAN_CLASSIC_SURYA",
        "2": "INDIAN_CLASSIC_DHARMA",
        "3": "INDIAN_KINGS_SURYA",
        "4": "INDIAN_KINGS_DHARMA"
    }
    game_name = modes.get(mode_choice, "INDIAN_CLASSIC_SURYA")

    # Select Game Level
    try:
        level_input = input("Enter Level [1-10] (Default 1): ").strip()
        game_level = int(level_input) if level_input else 1
    except ValueError:
        print("[⚠️] Invalid integer. Defaulting level to 1.")
        game_level = 1

    # Select Player Type
    human_input = input("Is this a Human player? (y/n, Default y): ").strip().lower()
    is_human = False if human_input == 'n' else True

    # Get Player Username
    username = input("Enter Player Name (Default: TestPlayer): ").strip()
    if not username:
        username = "TestPlayer"

    # 2. Duplicate template and update with user variables
    payload = BASE_TEMPLATE.copy()
    payload["Name"] = game_name
    payload["Level"] = game_level
    payload["Human"] = is_human
    payload["Players"] = [username]

    return payload

def send_payload(payload_dict):
    if not os.path.exists(SERVER_SOCKET_PATH):
        print(f"[❌] Error: Server socket file not found at {SERVER_SOCKET_PATH}", file=sys.stderr)
        return

    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.settimeout(10000.0)

    try:
        sock.connect(SERVER_SOCKET_PATH)

        # 3. USE JSON MODULE: Safely convert dictionary to raw payload string
        json_payload_string = json.dumps(payload_dict, indent=2)

        print("\n[>] Sending generated JSON payload:")
        print(json_payload_string)

        sock.sendall(json_payload_string.encode('utf-8'))

        print("[*] Waiting for pipeline response...")
        response = sock.recv(4096)

        if not response:
            print("[-] Server cut connection without data.")
        else:
            print(f"[+] Server Response: {response.decode('utf-8')}")

    except socket.timeout:
        print("[⚠️] Timeout. Server is likely parked waiting for a full game room.")
    except Exception as e:
        print(f"[❌] Exception: {e}", file=sys.stderr)
    finally:
        sock.close()

if __name__ == "__main__":
    print("[*] Z6 Template Payload Engine Initiated.")

    while True:
        custom_payload = get_user_inputs()
        send_payload(custom_payload)

        cont = input("\nDo you want to send another custom payload? (y/n): ").strip().lower()
        if cont != 'y':
            break

    print("[*] Testing session terminated.")
