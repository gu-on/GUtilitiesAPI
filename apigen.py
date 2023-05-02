import sys


def parse_header_file(header_file):
    with open(header_file, 'r') as file:
        lines = file.readlines()

    comment = ''
    decl = ''
    api_calls = []

    for i, line, in enumerate(lines):
        line = line.strip()

        if not line:  # ignore empty lines
            continue

        if line.startswith('#'):  # ignore #includes
            continue

        if line.startswith('//'):
            if comment:  # add empty space to multiline comments
                comment += ' '
            comment += line.lstrip('//').strip()
            continue

        if decl:  # add empty space to multiline declarations
            decl += ' '
        decl += line.strip()

        if line.endswith(';'):
            # extract params
            start = decl.find('(')
            end = decl.find(')')

            params = decl[start+1:end]

            param_types = ''
            param_names = ''

            if params:
                # Split the string into a list of parameter strings
                param_list = params.split(',')

                # Extract the types and variable names from each parameter string
                types = []
                names = []

                for param in param_list:
                    parts = param.split()
                    types.append(' '.join(parts[:-1]))
                    names.append(parts[-1])

                # Combine the types and names into two comma-separated strings
                param_types = ','.join(types)
                param_names = ','.join(names)

            # extract func_name
            end = decl.find('(')
            start = decl[:end].rfind(' ')
            func_name = decl[start+1:end]
            ret_type = decl[0:start]

            # print(f"Func Name: {func_name}\nRet Type: {ret_type}\nParams: {params}\nParam Types: {param_types}\nParam Names: {param_names}\nHelp: {comment}")

            api_calls.append({
                'help': comment,
                'ret_type': ret_type,
                'func_name': func_name,
                'param_types': param_types,
                'param_names': param_names,
            })
            comment = ""
            decl = ""

            # print("\n")

    file.close
    return api_calls


def generate_code(api_calls):
    code = []
    for call in api_calls:
        code.append(
            f'Api.Add({{APIFUNC({call["func_name"]}), "{call["ret_type"]}", "{call["param_types"]}", "{call["param_names"]}", "{call["help"]}"}});'
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


header_file = sys.argv[1]
main_cpp_file = sys.argv[2]

api_calls = parse_header_file(header_file)
output_code = generate_code(api_calls)
update_main_cpp_file(main_cpp_file, output_code)
