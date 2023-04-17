def parse_header_file(header_file):
    with open(header_file, 'r') as file:
        lines = file.readlines()

    api_calls = []

    for i, line in enumerate(lines):
        line = line.strip()
        if not line:  # Ignore empty lines
            continue

        if line.startswith('//'):
            description = line.lstrip('//').strip()
            func_line = lines[i + 1].strip()
            tokens = func_line.split()
            index = 0

            # Extract return type
            ret_type = ''
            while '*' in tokens[index] or tokens[index].isalpha():
                ret_type += tokens[index] + ' '
                index += 1
            ret_type = ret_type.strip()

            # Extract function name
            func_name = tokens[index].split('(')[0]
            index += 1

            # Extract parameters
            params = func_line[func_line.find(
                '(') + 1:func_line.find(')')].split(',')
            params = [param.strip() for param in params if param.strip()]
            param_types = ','.join([' '.join(p.split()[:-1]) for p in params])
            param_names = ','.join([p.split()[-1] for p in params])

            api_calls.append({
                'description': description,
                'ret_type': ret_type,
                'func_name': func_name,
                'params': params,
                'param_types': param_types,
                'param_names': param_names,
            })
    file.close()
    return api_calls


def generate_code(api_calls):
    code = []
    for call in api_calls:
        code.append(
            f'Api.Add({{APIFUNC({call["func_name"]}), "{call["ret_type"]}", "{call["param_types"]}", "{call["param_names"]}", "{call["description"]}"}});'
        )

    return '\n'.join(code)


def update_main_cpp_file(main_cpp_file, generated_code):
    with open(main_cpp_file, 'r') as file:
        lines = file.readlines()

    start_marker = "int LoadPlugin(reaper_plugin_info_t* rec)"
    end_marker = "Api.Register();"

    start_index = None
    end_index = None

    for i, line in enumerate(lines):
        if start_marker in line:
            start_index = i
        if end_marker in line:
            end_index = i
            break

    if start_index is not None and end_index is not None:
        # Remove the existing code between the markers
        del lines[start_index + 1:end_index]

        lines.insert(start_index + 1, "{\n")
        # Insert the generated code between the markers
        for i, line in enumerate(generated_code.split('\n')):
            lines.insert(start_index + 2 + i, '\t' + line + '\n')

        # Write the updated lines to the main.cpp file
        with open(main_cpp_file, 'w') as file:
            file.writelines(lines)

    file.close()


main_cpp_file = 'source/main.cpp'
header_file = 'include/api_cfg.hpp'

api_calls = parse_header_file(header_file)
output_code = generate_code(api_calls)
update_main_cpp_file(main_cpp_file, output_code)
