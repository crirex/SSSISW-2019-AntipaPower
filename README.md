# SSSISW-2019-AntipaPower
Team Name: AntipaPower <br />
Team members: <br />
<ul>
<li>Anton Gabriel</li>
<li>Balan Cristian</li>
<li>Hanganu Bogdan</li>
<li>Radu Lucian Andrei</li>
</ul>

# Help for installation
<ul>
<li>1)First you need to do a directory (optionally named AMMO, in which you will do the next steps)</li>
<li>2)Clone the repo in this folder</li>
<li>3)Do this tutorial http://api.projectchrono.org/tutorial_install_chrono.html <br />
(When extracting Irrlicht, put it in the AMMO directory) <br />
(When building the chrono use a Win64 build)</li>
<li>4) Go inside the script folder and use CMakeRegenerate.bat and it should do everything. If it dosen't work, then use Alternate Step 4) <br />
( Alternate Step 4) Use cmake on the repo to build it and put(Edit the value of Chrono_Dir to ${the_path_to_AMMO_project}/chrono_build/cmake, where the_path_to_AMMO_project is your actual path to the project))</li>
</ul>
# Additional info:
-Do not copy builded directories from one place to another, rebuild them in the directory needed.




