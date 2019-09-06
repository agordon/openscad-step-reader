# openscad-step-reader

A proof-of-concept project, testing OpenSCAD / STEP files loading.

## Usage


    $ openscad-step-reader
    
    A proof-of-concept program for STEP/OpenSCAD integration
    
    usage: openscad-step-reader [options] INPUT.STEP
    
    Output is written to STDOUT.
    
    options are:
       -h, --help         this help screen
       -V, --version      version information
    
       -o, --stl-occt     convert the input STEP file into ASCII STL file
                          using OpenCASCADE code. This should be the baseline
                          when debugging/troubleshooting incorrect outputs.
    
       -a, --stl-ascii    convert the input STEP file into custom ASCII STL file,
                          using our code. This is a good test to check mesh
                          triangulation code. EXCEPT for the 'normal' values
                          which are not produced, the vertex values should be
                          equivalent to those with --stl-occt.
    
       -s, --stl-scad     convert the input STEP file into SCAD code, containing
                          a single 'polyhedron' call with the STL triangles stored
                          in SCAD vectors.
    
       -f, --stl-faces    convert the input STEP file into SCAD code, retaining the
                          'face' information from the STEP file. Each face will be rendered
                          in a different color in openscad $preview mode.
    
       -e, --explore      Work-in-progress code, used for development and exploration
                          of OpenCASCADE class hierarchy, e.g.
                          Shell->Face->Surface->Wire->Edge->Vertex.
                          produces debug messges and no useful output.


## Examples

See the [examples] sub directory for STEP files and their STL/SCAD output.

e.g. the files in `examples/box` were created with:

    openscad-step-reader --stl-ascii examples/box/box.stp > examples/box/box-ascii.stl
    openscad-step-reader --stl-scad  examples/box/box.stp > examples/box/box.scad
    openscad-step-reader --stl-faces examples/box/box.stp > examples/box/box-faces.scad


The `--explore` option is a development tool to help learn
and understand the OpenCASCADE hierarchy class model (e.g.
shape->shell->face->surface->wire->edge->vertex).

    $ openscad-step-reader --explore example/cylinder/cyl.stp

    Shell 0
      Face 0 Type = 1:Cylinder
        Wire 0
         edge 0
           VertexFirst 2, -4.89859e-16, 10
           VertexLast 2, -4.89859e-16, 10
         edge 1
           VertexFirst 2, -4.89859e-16, 0
           VertexLast 2, -4.89859e-16, 10
         edge 2
           VertexFirst 2, -4.89859e-16, 0
           VertexLast 2, -4.89859e-16, 0
         edge 3
           VertexFirst 2, -4.89859e-16, 0
           VertexLast 2, -4.89859e-16, 10
      Face 0 Type = 0:Plane
        Wire 0
         edge 0
           VertexFirst 2, -4.89859e-16, 10
           VertexLast 2, -4.89859e-16, 10
      Face 0 Type = 0:Plane
        Wire 0
         edge 0
           VertexFirst 2, -4.89859e-16, 0
           VertexLast 2, -4.89859e-16, 0


The `--stl-scad` converts to STEP to triangles (just like an STL file),
then writes the triangles as openSCAD code:

    $ openscad-step-reader --stl-scad example/box/box.stp
    points = [
      [0,10,0],[0,0,0],[0,0,10], // Triangle 1 / 12
      [0,10,10],[0,10,0],[0,0,10],
      [10,0,10],[10,0,0],[10,10,0],
      [10,0,10],[10,10,0],[10,10,10],
      [0,0,0],[10,0,0],[10,0,10],
      [0,0,10],[0,0,0],[10,0,10],
      [10,10,10],[10,10,0],[0,10,0],
      [10,10,10],[0,10,0],[0,10,10],
      [0,0,0],[0,10,0],[10,10,0],
      [10,0,0],[0,0,0],[10,10,0], // Triangle 10 / 12
      [10,10,10],[0,10,10],[0,0,10],
      [10,10,10],[0,0,10],[10,0,10],
    ];
    faces = [
      [0,1,2], // Triangle 1 / 12
      [3,4,5],
      [6,7,8],
      [9,10,11],
      [12,13,14],
      [15,16,17],
      [18,19,20],
      [21,22,23],
      [24,25,26],
      [27,28,29], // Triangle 10 / 12
      [30,31,32],
      [33,34,35],
    ];
    module solid_object() {
      polyhedron (points,faces);
    }

    solid_object();


## License

Written by Assaf Gordon (assafgordon@gmail.com)
License: LGPLv2.1 or later
