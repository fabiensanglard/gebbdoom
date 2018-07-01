import sys

class CallSite:
    def __init__(self):
        self.calls = {}
        self.fctName = ""
        self.count = 0

    def finish(self):
        self.count += 1

class FlameGraph:
    def __init__(self):
        self.root = CallSite()

    def add(self, callStack):
        if len(callStack) == 0:
            return
        callsite = self.root
        for call in reversed(callStack):
            if call not in callsite.calls.keys():
                callsite.calls[call] = CallSite()
                callsite.fctName = call
            callsite = callsite.calls[call]
        callsite.finish()



def addCall(callstack, line):
    if len(line) == 0 or line[0] != '#':
        return
    tokens = line.split()
    funcName = tokens[1]
    if "[" in funcName:
        funcName += " " + tokens[2]
    if funcName[0] == '0' and funcName[1] == 'x':
        funcName = tokens[3]
        if "[" in funcName:
            funcName += " " + tokens[4]
    callstack.append(funcName)

def traverseStacks(stack, callsite, spaces):
    if callsite.count != 0:
        calls = ""
        calls = ";".join(stack)
        # calls += callsite.fctName + ";"
        calls += " " + str(callsite.count)
        print calls

    for key in callsite.calls.keys():
        stack.append(key)
        traverseStacks(stack, callsite.calls[key], spaces + "  ")
        stack.pop()

def isBackTraceStart(line):
    return line[0] == '#' and line[1] == '0'

def run():

    with open("gdb.txt") as f:
        content = f.readlines()

    content = [x.strip() for x in content]
    flameGraph = FlameGraph()
    callstack = []
    for line in content:
        if len(line) == 0 or line[0] != '#':
            continue

        if isBackTraceStart(line):
            flameGraph.add(callstack)
            callstack = []
        addCall(callstack, line)

    traverseStacks([], flameGraph.root, "")

run()



