import hou
import json
import pprint
import sys


def ReadJson(jsonFile):
    with open(jsonFile, "r") as f:
        return json.load(f)
    
def OpenHipFile(path):
    # if hou.isUIAvailable():
    hipPath = hou.hipFile.path()
    if hipPath != path:
        hou.hipFile.load(path, ignore_load_warnings = True)

# note:for run this need a main window to contain script window(which mean need open houdini)
def SelecteFile():
    if hou.isUIAvailable():
        return hou.ui.selectFile()
    else:
        print("can not use the ui")

def RenNode(nodes):
    for index, nodeStr in enumerate(nodes) :
        renNode = hou.node(nodeStr)
        print("processing node:{current}/{total}".format(current = index+1, total = len(nodes)), nodeStr)
        if renNode:
            nodeType = renNode.type()
            if(nodeType.name() == "ifd" or nodeType.name() == "usdrender_rop" or nodeType.name() == "rop_geometry"):
                # print(renNode.type())
                # print(renNode.name())
                try:
                    renNode.render()
                except hou.OperationFailed as e:
                    print("failed to render:"+renNode.name())
                    continue
            else:
                print("the node is not a avaliable type:" + renNode.name())

def StartRen(hipPath, renNodes):
    OpenHipFile(hipPath)
    RenNode(renNodes)
