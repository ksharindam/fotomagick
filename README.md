# FotoMagick Photo Editor
A Photo editor program based on graphicsmagick and opencv with Qt4 gui  

Why not GIMP?  
gimp has many unnecessary features. only few is required  
gimp has higher start up time.  
gimp is not user friendly.  
it does not have add frame feature.  

## Build
**dependency :**  
* libgraphicsmagick++1-dev  
* libopencv-core-dev  
* libopencv-imgproc-dev  

To build type the following commands ...  
`qmake`  
`make -j4`

## My planned features...
scissor selection tool  
forground extraction tool  
rectangle selection tool  
free hand selection tool with edge snapping.  

increase/decrease selection boundary  

Color:  
    Auto white balance  
    Auto equalize  
    Auto contrast  

Filters:  
    Pencil Sketch  

Studio:  
    Skin Whiten  
    Remove spots  

Repair:  
    Dust reduction  
    Smudge tool  

Workspaces:   
    Scanned Photo  
    Beautify Face  
    Artistics  
    Decorate (frame)  

Selection Tools:  
    Intelligent Scissor  
    Color Selection Tool  
    Freehand with edge snap  
    Rectangle  
    Forground object extraction  

Change Background  
Photo Stiching  

## Todo
add all tool buttons
add smooth brush
Layer and mask manager
swap fg and bg colors