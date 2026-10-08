# HouBackRender

### Download

download the release binaries and the **Script** folder or clone this lib: *git clone --recursive https://github.com/underwatersponge/HouBackRender.git*
run **GenProj.bat** to generate projection

### Using

copy the **python folder** in *Script/PythonScrip you just download* to your HOUDINI_PATH/scripts or HOUDINI_PATH/pythonX.Ylibs [see detail](https://www.sidefx.com/docs/houdini/hom/locations.html) and if you are playing houdini,restart houdini
put the **.exe and NodeGet.py testRen.py** to folder which you like

##### About the ui

it just generate a .json file save hip file and node to render that python script will use,generate a bat file to run python,and generate a .json file to save the houdini bin folder path.after generate them,you can run the .bat file to start render...

##### TODO maybe in future

here have a python file use for houdini shelf tool,it can generate json file to save hipfile can render node in houdini,but it not finished now,just need a few minute you can write it by yourself it is sample

### About Shader fun

you can click the **MenuItem playshader** to see it,you can edit the fragment shader in code edit area **Ctrl + R** to run here have some example shader in folder *ShadeerExample*,you can find more interesting shader in <https://www.shadertoy.com/>.

