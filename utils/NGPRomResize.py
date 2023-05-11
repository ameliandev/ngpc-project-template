import sys
import os

rom2MbSize = 2097152


def resize_rom(data, new_file_name, path):
    rom_length = len(data)
    new_data = bytearray([0]) * rom2MbSize

    for i in range(rom2MbSize):
        if i >= rom_length:
            new_data[i] = 0
        else:
            new_data[i] = data[i]

    try:
        if os.path.exists(os.path.join(path, new_file_name)):
            os.remove(os.path.join(path, new_file_name))
    except Exception:
        print(
            f"Error deleting old file {os.path.join(path, new_file_name)}. Try to delete it manually")
        return

    try:
        with open(os.path.join(path, new_file_name), "wb") as file:
            file.write(new_data)

        print("File resized! Check the same rom path")
    except Exception as ex:
        print(
            f"Error creating new file {os.path.join(path, new_file_name)}. Error: {ex}")


def main():
    if len(sys.argv) <= 1:
        print("ROM route not indicated")
        return

    rom_file = sys.argv[1]
    file_name = os.path.basename(rom_file)
    file_path = os.path.dirname(os.path.realpath(__file__))

    if "\\" in rom_file:
        file_parts = rom_file.split("\\")
        file_name = file_parts[-1]
        file_path = rom_file.replace(file_name, "")

    try:
        with open(rom_file, "rb") as file:
            data = file.read()

        if len(data) >= rom2MbSize:
            print(
                f"File size of {file_name} is equal or bigger than {rom2MbSize}. Resize not needed")
        else:
            resize_rom(data, f"2MB_{file_name}", file_path)
    except Exception as ex:
        print(f"Error reading ROM file: {ex}")


if __name__ == "__main__":
    print("Neo Geo Pocket - 2MB Rom Resizer - By Adrián Melián")
    main()
