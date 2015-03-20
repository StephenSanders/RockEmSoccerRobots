========================================================================
XAnimator Library

Created by Keith Ditchburn 2010 - 2013

www.toymaker.info
========================================================================

What is XAnimator?
------------------
XAnimator is a small library to allow the loading, animating and rendering of Direct3D .x files
As well as loading and rendering it provides access to bounding shapes including the lowest 
level mesh bounding shapes calculated according to the animation state of the model

Since animated models cannot be re-instanced (unless you want them all to be in the same 
animation state) you need to load a model for each world instance. This is not so wasteful
however as internally XAnimator reuses mesh data across models with the same file name. This 
massively reduces the amount of data held per additional model.

Limitations
-----------
- in the event of a D3D device change, the library needs to be closed down and restarted
- skinning is supported but only in software
- effect files are not yet supported

Possible Future Additions
-------------------------
- Support for effect files
- Hardware skinning
- Provide access to the frame hierarchy

Useage
------
For a detailed description of how to use the library please see the web site.

Firstly you need too include the XAnimator_lib.h file and link to the correct lib file (see below).
To create XAnimator you then call CreateAnimiX with a valid Direct3D device pointer. This then returns
an instance pointer for the XAnimator system. You only need one of these. This pointer then provides 
functionality to load an .x file, render it, change animation set and get bounding shapes. When you load 
a model you are returned a unique model id that you use in later calls to the system.

Files
-----

XAnimator_lib.h - All the includes required to use the API 

Your application must be a Windows API application and you should link with one of 
the XAnimator static libraries plus the DirectX libraries as listed below:

Release Configuration
---------------------
Link with: XAnimator_32.lib for 32 bit builds or XAnimator_64.lib for 64 bit ones
Plus the following DirectX library files: dxguid.lib dxerr.lib d3d9.lib d3dx9.lib

Debug Configuration
-------------------
Link with: XAnimator_d32.lib for 32 bit builds or XAnimator_d64.lib for 64 bit ones
Plus the following DirectX library files: dxguid.lib dxerr.lib d3d9.lib d3dx9d.lib

Note: XAnimator uses the DLL versions of the C++ runtime libraries (/MDd or /MD)

Change Log
----------------------------------------------
Last modified by Keith Ditchburn: 17th February 2013

Version 1.00
- If a texture is not found in the path indicated by the mesh it is looked for in the working directory
If the mesh is then saved this filename is saved rather than the original path
If the texture still cannot be found the texture filename is retained
- Fixed a memory leak
- Added a call to discard a model
- You can now provide an interface instance to get XAnimator debug output
- added flag XANIMATOR_NO_CACHE_MESH to prevent an XAnimator optimisation where the same mesh are opnly loaded once
- fixed a problem when saving the .x file the animation set ids were reversed
- XAnimator no longer adds a root frame by default when saving
- added Extended/Advanced functions in order to provide external access to the model data
	ExGetHierarchy and ExChangeHierarchy
- added ExTransformRoot to apply a transformation to the root frame of the model

Version 0.94 07/02/13
- Rebuilt with Visual Studio 2012 

Version 0.93 23/3/11
- Fixed a bug where GetAnimationSetName was ignoring the passed in index and just returning the current animation set

Version 0.92 19/11/10
- Fixed a crash when model has no animation data

Version 0.91
- Fixed a problem when no animation set was specified to start with causing a mix of animations
- Fixed a problem where in rare cases an incorrect world matrix was used for skinning

Version 0.90
- Fixed problems occuring when a model has multiple skinned mesh
- Rebuilt using June 2010 version of the DirectX SDK
- Rebuilt using Visual Studio 2010
- Added further debug output when in verbose mode

Version 0.85
- Fixed a freeze that happened in some cases 
- Ambient material is now set to the x files diffuse colour rather than black
- Fixed a crash when trying to save a model that does not have animation data
- Prevented a crash when querying for animation data for a model that does not have any
- Improved error checking for situations where a model does not have animation data
Version 0.8
- added functions GetNumberOfFramesInCurrentAnimationSet, GetCurrentAnimationFrame and
SetAnimationFrame to allow the frame number to be queiried and set
- fixed a problem in the saving causing issues with some .x files

Version 0.7
- Added a AddAnimationsFromXFile function to allow animations to be combined from other 
x files with the same structure
- Added SaveModel to all the model to be saved in text or binary format
- Fixed a Direct3D memory leak introduced in 0.6

Version 0.6
- Fixed a bug in skinned mesh rendering when the skinned mesh was not the first in the hierarchy
- Fixed a bug causing the 8th corner returned by GetBoundingShapes to be wrong
- Added a function ChangeAnimationPlaybackSpeed to allow animation speed to be scaled
- Changed so verbose mode is false by default
- Added flags to LoadXFile function to allow mesh optimisations
- Added support for 32 bit index buffers
- Added more debug output when verbose is on including a frame / mesh hierarchy
- Various optimisations

Version 0.5 02/02/10
- Changed to pass time as float 
- Changed name
- Removed redundant code
- Optimisations

Version 0.4 31/01/10
- Finalised the interface and thoroughly tested
- Combined bounding shape calculation and access into one function

Version 0.3 26/01/10
- Added function to retrieve individual mesh bounding boxes

Version 0.1 18/01/10
- Initial builds