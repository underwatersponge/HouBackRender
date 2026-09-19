from NodeSearch import NodeSearch 
import hou
import pprint
from dataclasses import dataclass
# cache File

@dataclass
class SearchInfo:
    # seems because python argument passing this default value is not every 
    # instance give one,i am not sure,if you know tell me
    # https://docs.python.org/3/faq/programming.html#how-do-i-write-a-function-with-output-parameters-call-by-reference
    # https://docs.python.org/3/faq/programming.html#why-are-default-values-shared-between-objects
    # however i think here will not get a bug
    cats:list # = [hou.sopNodeTypeCategory(),hou.objNodeTypeCategory()]
    network:hou.Node = hou.node("/obj")
    typeName:str = ""
    exact:bool = True
    recursive:bool = True
    limiteInput:bool = False
    allowPassFlag:bool = False

def IsAllowRen(node, limiteInput = False, allowPassFlag = False):
    # if(node):
    numInput = len(node.inputs())
    bPassFlag = node.isGenericFlagSet(hou.nodeFlag.Bypass)
    if(limiteInput and numInput < 1):
        return False
    if(not allowPassFlag and bPassFlag):
        return False
    return True

# TODO: a better function name
def GetMantra(parentNode = hou.node("/out"), limiteInput = False, allowPassFlag = False):
    result = []
    for node in parentNode.children():
        if node.type().name() == "ifd":
            bAllowRen = IsAllowRen(node, limiteInput, allowPassFlag)
            if(bAllowRen):
                result.append(node.path())
    return result

# TODO: a better function name
def GetKarma(parentNode = hou.node("/stage"), limiteInput = True, allowPassFlag = False):
    result = []
    for node in parentNode.children(): 
        if node.type().name() == "usdrender_rop":
            bAllowRen = IsAllowRen(node, limiteInput, allowPassFlag)
            if(bAllowRen):
                result.append(node.path())
    return result

def GetCacheNode(searchInfo = SearchInfo(cats = [hou.sopNodeTypeCategory()],network = hou.node("/obj"),
                                         typeName = "filecache::2.0", limiteInput = True, allowPassFlag = False)):
    result = []
    containerNodes = NodeSearch.SearchNodes(searchInfo.network, searchInfo.typeName,searchInfo.cats,searchInfo.exact,searchInfo.recursive)
    for nodepath in containerNodes:
        node = hou.node(nodepath)
        numInput = len(node.inputs())
        bPassFlag = node.isGenericFlagSet(hou.nodeFlag.Bypass)
        bAllowRen = IsAllowRen(node, searchInfo.limiteInput, searchInfo.allowPassFlag)
        if(bAllowRen):
            result.append(nodepath + "/render")
    return result

def GetMantraNode(searchInfo = SearchInfo(cats = [hou.objNodeTypeCategory(), hou.sopNodeTypeCategory()],network = hou.node("/obj"),
                                          typeName = "ropnet", limiteInput = False,allowPassFlag = False)):
    if not searchInfo.typeName:
        searchInfo.typeName = "ropnet"
    result = []
    # custome create
    containerNodes = NodeSearch.SearchNodes(searchInfo.network, searchInfo.typeName, searchInfo.cats, searchInfo.exact, searchInfo.recursive)
    for nodepath in containerNodes:
        containerNode = hou.node(nodepath)
        temp = GetMantra(containerNode, searchInfo.limiteInput, searchInfo.allowPassFlag)
        result.extend(temp)           
    # default create(/out)
    temp = GetMantra(limiteInput=searchInfo.limiteInput, allowPassFlag = searchInfo.allowPassFlag)
    result.extend(temp)

    return result

# TODO:not to check the  usdrender_rop flag but setting node of it
def GetKarmaNode(searchInfo = SearchInfo(cats = [hou.objNodeTypeCategory(), hou.sopNodeTypeCategory()],network = hou.node("/obj"),
                                          typeName = "lopnet", limiteInput = True,)):
    result = []
    # custom create
    containerNodes = NodeSearch.SearchNodes(searchInfo.network, searchInfo.typeName, searchInfo.cats, searchInfo.exact, searchInfo.recursive)
    for nodepath in containerNodes:
        containerNode = hou.node(nodepath)
        temp = GetKarma(containerNode, searchInfo.limiteInput, searchInfo.allowPassFlag)
        result.extend(temp)
    # default create(/stage)
    temp = GetKarma(limiteInput=searchInfo.limiteInput, allowPassFlag = searchInfo.allowPassFlag)
    result.extend(temp)

    return result

def GetAllRenNode():
    result = GetCacheNode()
    result.extend(GetMantraNode())
    result.extend(GetKarmaNode())
    return result
