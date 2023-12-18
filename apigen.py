import sys

def ParseHeaderFile(headerFile):
    lines = []
    
    with open(headerFile, 'r') as file:
        lines = file.readlines()

    comment = ''
    decl = ''
    apiCalls = []

    for line in lines:
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

            paramTypes = ''
            paramNames = ''

            if params:
                # Split the string into a list of parameter strings
                paramList = params.split(',')

                # Extract the types and variable names from each parameter string
                types = []
                names = []

                for param in paramList:
                    parts = param.split()
                    types.append(' '.join(parts[:-1]))
                    names.append(parts[-1])

                # Combine the types and names into two comma-separated strings
                paramTypes = ','.join(types)
                paramNames = ','.join(names)

            # extract func_name
            end = decl.find('(')
            start = decl[:end].rfind(' ')
            FuncName = decl[start+1:end]
            retType = decl[0:start]

            apiCalls.append({
                'help': comment,
                'ret_type': retType,
                'func_name': FuncName,
                'param_types': paramTypes,
                'param_names': paramNames,
            })

            comment = ""
            decl = ""

    file.close
    return apiCalls

def GenerateCode(apiCalls):
    code = []
    for call in apiCalls:
        code.append(
            f'\tApi.Add({{APIFUNC({call["func_name"]}), "{call["ret_type"]}", "{call["param_types"]}", "{call["param_names"]}", "{call["help"]}"}});'
        )

    return '\n'.join(code)

def RunAPIGen(mainCppFile, generatedCode):
    startBlock = [
        '#define REAPERAPI_IMPLEMENT',
        '#include <reaper_plugin_functions.h>',
        '',
        '#include <api_manager.hpp>',
        '#include <gu_api.hpp>',
        '#include <gu_ini_file.hpp>',
        '',
        'API Api{};',
        '',
        'void LoadPlugin()',
        '{'
    ]
    
    endBlock = [
        '	Api.Register();',
        '}',
        '',
        'extern "C"',
        '{',
        '	REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance [[maybe_unused]], reaper_plugin_info_t* rec)',
        '	{',
        '		if (rec && REAPERAPI_LoadAPI(rec->GetFunc) == 0)',
        '		{',
        '			INIFile ini{"GUtilities"};',
        '#ifdef _DEBUG',
        '			ini.Write("debug", "enabled", "true");',
        '#else',
        '			ini.Write("debug", "enabled", "false");',
        '#endif',
        '			LoadPlugin();',
        '			return 1;',
        '		}',
        '		else',
        '		{',
        '			Api.Unregister();',
        '			return 0;',
        '		}',
        '	}',
        '}'
    ]

    # Write the updated lines to the gu_main.cpp file
    with open(mainCppFile, 'w') as file:
        for line in startBlock:
            file.write(line + "\n")
        for line in generatedCode:
            file.write(line)
        file.write("\n")
        for line in endBlock:
            file.write(line + "\n")

    file.close()

headerFile = sys.argv[1]
mainCppFile = sys.argv[2]

apiCalls = ParseHeaderFile(headerFile)
outputCode = GenerateCode(apiCalls)
RunAPIGen(mainCppFile, outputCode)
