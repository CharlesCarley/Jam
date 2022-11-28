import sys
import os
import subprocess
import shutil

class Path:

    def __init__(self, directory=None):
        if (directory != None):
            self.path = directory
            self.normalize()
        else:
            self.path = os.getcwd()
            self.normalize()

        if (not self.check()):
            msg = "The supplied path %s is invalid" % self.path
            raise Exception(msg)

    def normalize(self):
        if (sys.platform == "win32"):
            self.path = self.path.replace('/', '\\')
        else:
            self.path = self.path.replace('\\', '/')
        self.path = os.path.abspath(self.path)

    def check(self):
        return os.path.exists(self.path)

    def __repr__(self) -> str:
        return self.path

    def back(self, n=1):

        back = self.path
        for i in range(n):
            back = os.path.join(back, "../")

        return Path(back)

    def join(self, directory):
        result = self.path
        return Path(os.path.join(result, directory))

    def file(self, path):
        result = self.path
        return os.path.join(result, path)

    def create(self, relative):
        result = self.path
        if (sys.platform == "win32"):
            relative = relative.replace('/', '\\')
        else:
            relative = relative.replace('\\', '/')

        joinResult = os.path.join(result, relative)
        if (not os.path.isdir(joinResult)):
            os.makedirs(joinResult)
        return Path(joinResult)

    def subdir(self, relative):
        result = self.path
        if (sys.platform == "win32"):
            relative = relative.replace('/', '\\')
        else:
            relative = relative.replace('\\', '/')

        joinResult = os.path.join(result, relative)
        if (not os.path.isdir(joinResult)):
            msg = "The path '%s' does not exist " % joinResult
            raise Exception(msg)

        return Path(joinResult)

    def recreate(self):
        result = self.path

        os.makedirs(result)
        return Path(result)

    def remove(self):
        if (os.path.isdir(self.path)):
            print("Removing".ljust(20), "=> ", self.path)
            shutil.rmtree(self.path, ignore_errors=True)

    def copyTo(self, file, toPath):
        print("Copy", self.file(file), "=> ", toPath.file(file))

        shutil.copyfile(self.file(file), toPath.file(file))
        shutil.copymode(self.file(file), toPath.file(file))

    def copyTree(self, toPath):
        print("Copy".ljust(20), "=> ", toPath.path)
        shutil.copytree(self.path, toPath.path, dirs_exist_ok=True)

    def removeFile(self, file):
        localFile = os.path.join(self.path, file)

        if (os.path.isfile(localFile)):
            print("Removing", localFile)
            os.remove(localFile)


class Builder:

    def __init__(self, argc, argv):

        self.argc = argc
        self.argv = argv
        self.release = self.findOpt("--release")
        self.opts = {}

        sourceDir = Path()
        self.opts['source'] = sourceDir
        self.opts['build'] = sourceDir.create("Build")
        self.opts['deploy'] = sourceDir.create("Bin")
        self.opts['test'] = sourceDir.join("Testing")

        if (sys.platform == "win32"):
            platName = "windows"
        else:
            platName = 'linux'

        self.opts['platform'] = platName

    def home(self): return self.opts['source']
    def sourceDir(self): return self.opts['source']
    def buildDir(self): return self.opts['build']
    def deployDir(self): return self.opts['deploy']

    def dumpOpts(self):
        print("")
        print("Build Paths")
        for k in self.opts.keys():
            print(k.ljust(20), "=>", self.opts[k])
        print("")

    def goto(self, path):
        try:
            os.chdir(path.path)
        except:
            msg = "Failed to change working directory to %s" % path.path
            raise Exception(msg)

    def configString(self):
        config = "Debug"
        if (self.release):
            config = "Release"
        return config

    def run(self, cmd):
        print("Calling =>", cmd)
        subprocess.run(cmd, shell=True, env=os.environ)

    def findOpt(self, opt):
        for i in range(self.argc):
            if (opt == self.argv[i]):
                return True
        return False

    def clean(self, reCreate=False):
        print("Cleaning...".ljust(20), self.argv)
        self.buildDir().remove()
        self.goto(self.home())

    def configure(self, reCreate=False):
        print("Configure...".ljust(20), self.argv)

        self.goto(self.buildDir())
        self.run("cmake ..")
        self.goto(self.home())

    def build(self):
        print("Building...".ljust(20), self.argv)
        self.goto(self.buildDir())

        self.run("cmake .. -DJam_BUILD_TEST=ON -DJam_AUTO_RUN_TEST=ON")
        self.run("cmake --build %s "%(self.buildDir()))
        self.goto(self.home())

    def deploy(self ):
        print("Deploy...".ljust(20), self.argv)
        self.goto(self.deployDir())
        self.run("windeployqt .")
        self.goto(self.home())
  
    def edit(self ):
        print("Testing...".ljust(20), self.argv)
        self.goto(self.home())

    def logUsage(self):
        print("build <kind> <options>")
        print("")
        print("  Where <kind> is one of the following")
        print("")
        print("  clean  - Removes the build directories")
        print("  config - Configure with CMake")
        print("  help   - Displays this message")
        print("  deploy - Build the windows deployment")
        print("")
        print("")


def main(argc, argv):
    build = Builder(argc, argv)
    build.dumpOpts()
    build.logUsage()

    if (build.findOpt("clean")):
        build.clean();
    elif (build.findOpt("config")):
        build.configure();
    elif (build.findOpt("deploy")):
        build.deploy();
    else:
        build.build();


    build.goto(build.home())

if __name__ == '__main__':
    main(len(sys.argv), sys.argv)
