from base64 import encode
import sys
from OpenGL import Enabled as GL

WS = "".ljust(4, " ")
NL = "\n"

# import requests
# def fetchDocs(name):
#    try:
#          r = requests.get('https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/%s.xml'%name)
#          if (r.status_code == 200):
#             print("/// <summary>")
#             print("/// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/%s.xml"%name)
#             print("/// </summary>")
#    except:
#        pass


class PrintUtils:
    @staticmethod
    def logHeader(key):
        return ""

    @staticmethod
    def logDefine(name):
        return "#define %s 1\n" % name

    @staticmethod
    def logIfDef(name):
        return "#ifdef %s\n" % name

    @staticmethod
    def logEndif(name):
        return "#endif // %s\n" % name

    @staticmethod
    def space():
        return "\n"


class Definition:
    def __init__(self, name, specs):
        self.name = name
        self.name_len = len(name)
        self.value = specs[2]
        self.specs = specs


class Function:
    def __init__(self, name, specs):
        self.name = name
        self.specs = specs
        self.ret = specs[0]
        self.proc_name = "PFN" + name.upper() + "PROC"

        if self.ret.find("*") != -1:
            self.ret_code = "nullptr"
        elif self.ret == "void":
            self.ret_code = None
        else:
            self.ret_code = "(%s)0" % self.ret

        self.param_list = specs[2]
        self.param_count = len(specs[2])
        self.param_types = []
        self.param_names = []

        self.max_param_type = 0
        self.max_param_name = 0

        for param in specs[2]:
            self.max_param_type = max(self.max_param_type, len(param[0]))
            self.max_param_name = max(self.max_param_name, len(param[1]))

            self.param_types.append(param[0])
            self.param_names.append(param[1])


class Specification:
    def __init__(self, mgr, name, specs):
        self.name = name
        self.specs = specs
        self.definition = name.replace(".", "_").replace(" ", "_")
        self.functions = {}
        self.definitions = {}
        self.max_fn_name = 0
        self.max_param_type = 0
        self.max_param_name = 0
        self.max_def_name = 0
        self.max_def_val = 0
        self.max_ret_val = 0

        for i in range(0, len(specs)):
            func = specs[i]

            # Cache it in the global output namespace dictionary.
            if mgr.isNotGloballyUnique(func[1]):
                print("// " + self.definition + " ==> skipped duplicate " + func[1])
                continue

            if len(func) != 3:
                print("// Skipping malformed", func)

            elif func[0] == "define":
                gl_def = Definition(func[1], func)
                self.max_def_name = max(self.max_def_name, len(func[1]))
                self.max_def_val = max(self.max_def_val, len(func[2]))
                self.definitions[func[1]] = gl_def

            else:
                gl_func = Function(func[1], func)

                self.max_param_type = max(self.max_param_type, gl_func.max_param_type)
                self.max_param_name = max(self.max_param_name, gl_func.max_param_name)

                self.max_fn_name = max(self.max_fn_name, len(gl_func.name))
                self.max_ret_val = max(self.max_ret_val, len(func[0]))
                self.functions[func[1]] = gl_func

    def hasDefines(self):
        return len(self.definitions) > 0

    def printDefines(self):
        value = ""
        for definition in self.definitions.values():
            value += "#define "
            value += definition.name
            value += " ".ljust(self.getDefinePadding(definition), " ")
            value += definition.value
            value += "\n"
        return value

    def fixupName(self, name):

        if len(name) > 2:
            fixupName = name
            if fixupName[0:2] == "gl":
                fixupName = fixupName[2:]
            return fixupName[0:1].lower() + fixupName[1:]
        return name

    def printTypedef(self):
        value = ""
        for function in self.functions.values():
            subString = "typedef "
            subString += function.ret
            subString += "".ljust(self.getReturnPadding(function), " ")
            subString += "(GL3_APIENTRYP "
            subString += function.proc_name
            subString += ")("
            for i in range(len(function.param_types)):
                if i > 0:
                    subString += ", "
                subString += function.param_types[i]
            subString += ");"
            value += subString + "\n"
        return value


    def printPrototype(self):
        value = ""
        for function in self.functions.values():
            subString = "    extern "
            subString += function.ret + " "
            subString += "".ljust(self.getReturnPadding(function), " ")
            subString += self.fixupName(function.name)
            subString += "("
            for i in range(len(function.param_types)):
                if i > 0:
                    subString += ", "
                subString += function.param_types[i]
            subString += ");"
            value += subString + "\n"
        return value

    def printExternal(self):
        value = ""
        for func in self.functions.values():
            subString = "GL3_API "
            subString += func.proc_name
            subString += "".ljust(self.getFunctionPadding(func), " ")
            subString += func.name + ";"
            value += subString + "\n"
        return value

    def printLookupPrototypes(self):
        value = ""
        for function in self.functions.values():
            subString = "static "
            subString += function.proc_name
            subString += "".ljust(self.getFunctionPadding(function), " ")
            subString += "__" + function.name
            subString += "".ljust(self.getFunctionPadding(function), " ")
            subString += " = nullptr;"
            value += subString + "\n"
        return value

    def getFunctionPadding(self, func):
        return max(0, self.max_fn_name - len(func.name) + 1)

    def getReturnPadding(self, func):
        return max(0, self.max_ret_val - len(func.ret) + 1)

    def getDefinePadding(self, dfn):
        return max(0, self.max_def_name - dfn.name_len + 1)

    def printAssignSourceWrapper(self):
        value = ""
        for function in self.functions.values():
            subString = ""
            subString += function.proc_name
            subString += "".ljust(self.getFunctionPadding(function), " ")
            subString += function.name
            subString += "".ljust(self.getFunctionPadding(function), " ")
            subString += " = gl::"
            subString += self.fixupName(function.name) + ";"
            value += subString + "\n"
        return value

    def printSourceWrapper(self):

        value = ""
        for function in self.functions.values():

            subString = ""
            subString += function.ret
            subString += " "
            subString += self.fixupName(function.name) + "("

            for i in range(len(function.param_types)):
                if i > 0:
                    subString += ", "
                subString += function.param_types[i]
                subString += " "
                subString += function.param_names[i]
            subString += ")" + NL + "{" + NL
            subString += "    if (__" + function.name + " != nullptr)" + NL

            if function.ret_code != None:
                subString += WS + WS + "return __" + function.name + "("
            else:
                subString += WS + WS + "__" + function.name + "("

            for i in range(len(function.param_types)):
                if i > 0:
                    subString += ", "
                subString += function.param_names[i]
            subString += ");" + NL

            if function.ret_code == None:
                subString += WS + "else" + NL + WS

            subString += WS + "__DEBUG_TRACE_MISSING(%s);" % function.name + NL
            if function.ret_code != None:
                subString += WS + "return " + function.ret_code + ";" + NL
            subString += "}"
            value += subString + "\n\n"
        return value

    def printLookupSymbol(self):
        value = ""
        value += "static GLint Load_" + self.definition + "()\n"
        value += "{\n"
        value += "    bool result=true;\n"
        value += "    // clang-format off\n"

        for function in self.functions.values():
            subString = WS
            subString += "result = __Symbol("
            subString += function.proc_name + ","
            subString += "".ljust(self.getFunctionPadding(function), " ")
            subString += function.name + ")"
            subString += "".ljust(self.getFunctionPadding(function), " ")
            subString += "&& result;"
            value += subString + "\n"

        value += "    return result ? 1 : 0;\n"
        value += "    // clang-format on\n"
        value += "}\n\n"
        return value


class Application:
    def __init__(self, argc, argv):
        self.name = ""
        self.argc = argc
        self.argv = argv
        self.maxKey = 0
        self.srcTemplate = ""
        self.hdrTemplate = ""
        self.API = {}
        self.globals = {}
        for key in GL.keys():
            self.maxKey = max(self.maxKey, len(key))
            self.API[key] = Specification(self, key, GL[key])

    def loadTemplates(self, fileName):
        self.name = fileName

        fp = open("%s/OpenGLHeader.txt" % (self.getOption("--base")), "r")
        self.hdrTemplate = fp.read()
        fp.close()

        fp = open("%s/OpenGLSource.txt" % (self.getOption("--base")), "r")
        self.srcTemplate = fp.read()
        fp.close()

    def isNotGloballyUnique(self, name):
        if name in self.globals:
            return True
        self.globals[name] = 1
        return False

    def getApiSpecifications(self):
        return self.API.values()

    def getApiSpecification(self, name):
        try:
            return self.API[name]
        except:
            return None

    def writeSpecsHeader(self):
        v = ""

        specs = self.getApiSpecifications()
        for spec in specs:
            v += PrintUtils.logDefine(spec.definition)

        v += PrintUtils.space()
        for spec in specs:
            v += PrintUtils.logHeader(spec.name)
            v += PrintUtils.logIfDef(spec.definition)

            if spec.hasDefines():
                v += spec.printDefines()
                v += PrintUtils.space()

            v += PrintUtils.space()
            v += "namespace gl {\n"
            v += spec.printPrototype()
            v += "} // namespace gl\n"
            v += PrintUtils.space()
            v += PrintUtils.logEndif(spec.definition)

        return v

    def writeHeader(self):

        specs = self.getApiSpecifications()

        src = self.hdrTemplate
        src = src.replace("${capabilities}", self.writeCapabilities())
        src = src.replace("${specification}", self.writeSpecsHeader())

        print(src)

    def writeCapabilities(self):

        ret = ""
        specs = self.getApiSpecifications()
        for spec in specs:
            ret += "".ljust(3, " ") + " GLint CAP_" + spec.definition + ";\n"
        return ret

    def writeSpecsSource(self):
        v = ""

        specs = self.getApiSpecifications()
        for spec in specs:
            v += PrintUtils.logHeader(spec.name)
            v += PrintUtils.logIfDef(spec.definition)

            v += spec.printTypedef()
            v += spec.printLookupPrototypes()
            v += PrintUtils.space()

        v += "namespace gl {\n"

        for spec in specs:
            v += spec.printSourceWrapper()
            v += PrintUtils.space()

        v += "} // namespace gl\n"

        for spec in specs:
            v += spec.printAssignSourceWrapper()
            v += PrintUtils.space()

            v += spec.printLookupSymbol()
            v += PrintUtils.space()

            v += PrintUtils.logEndif(spec.definition)

        return v

    def writeLoad(self):
        v = ""
        specs = self.getApiSpecifications()
        for spec in specs:

            capDef = "cap.CAP_" + spec.definition
            v += "    " + capDef + " = "
            v += "    Load_" + spec.definition + "();\n"
            #v += "    if (" + capDef + ") {\n"
            #v += "        printf(\"Loaded " + capDef + "\\n\");\n" 
            #v += "    }\n"
        return v

    def writeSource(self):

        src = self.srcTemplate
        src = src.replace("${genname}", self.name)
        src = src.replace("${impl}", self.writeSpecsSource())
        src = src.replace("${load}", self.writeLoad())
        print(src)

    def getOption(self, opt, de=""):

        for i in range(0, self.argc):
            if opt == self.argv[i] and i + 1 < self.argc:
                var = self.argv[i + 1]
                if len(var) < 32:
                    return var
        return None


def sanitizeString(val):
    bts = bytes("ascii", "utf-8")
    for byte in bts:

        if not (
            byte >= ord("a")
            and byte <= ord("z")
            or byte >= ord("A")
            and byte <= ord("Z")
        ):
            return False
    return True


def main(argc, argv):

    app = Application(argc, argv)
    fileName = app.getOption("--file")
    if fileName == None or not sanitizeString(fileName):
        fileName = "Undefined"

    app.loadTemplates(fileName)

    cur_stdout = sys.stdout
    redirect = open("%s.h" % fileName, "w")
    sys.stdout = redirect
    app.writeHeader()
    redirect.close()

    redirect = open("%s.cpp" % fileName, "w")
    sys.stdout = redirect
    app.writeSource()
    redirect.close()
    sys.stdout = cur_stdout
    return 0


if __name__ == "__main__":
    exit(main(len(sys.argv), sys.argv))
