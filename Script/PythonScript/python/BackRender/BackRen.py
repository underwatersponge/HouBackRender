import hou
import json
import pprint
import sys

from BackRender.ColorForPrint import Col

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

def OnFrameCompleted(rop_node,render_event_type,time):
    if render_event_type == hou.ropRenderEventType.PostFrame:
        curFrame = hou.frame()
        print("{} finished rendering frame {}.".format(rop_node.name(), int(curFrame)))


def RenNode(nodes):
    for index, nodeStr in enumerate(nodes) :
        renNode = hou.node(nodeStr)
        print(Col.GREEN)
        print("processing node:{current}/{total}".format(current = index+1, total = len(nodes)), nodeStr)
        print(Col.DEFAULT)
        if renNode:
            if isinstance(renNode, hou.RopNode):
                renNode.addRenderEventCallback(OnFrameCompleted)
            nodeType = renNode.type()
            if(nodeType.name() == "ifd" or nodeType.name() == "usdrender_rop" or nodeType.name() == "rop_geometry"):
                # print(renNode.type())
                # print(renNode.name())
                try:
                    renNode.render()
                except hou.OperationFailed as e:
                    print(Col.RED)
                    print("failed to render:"+renNode.name())
                    print(Col.DEFAULT)
                    continue
            else:
                print(Col.RED)
                print("the node is not a avaliable type:" + renNode.name())
                print(Col.DEFAULT)

def StartRen(hipPath, renNodes):
    OpenHipFile(hipPath)
    RenNode(renNodes)

