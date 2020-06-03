## EXIF-To-MapQT

* TL;DR: UI for this project https://github.com/hntirgeam/EXIF-To-Map

* Simple C++ program with UI (based on QT framework) and python script that will create map with markers on it, by checking all your *.jpeg files for EXIF instancies.  

* The idea was to find out WHERE I took my photos and show it on the interactive world map. Parsing operation is running in sub-thread so UI won't be blocked by this operation. 

* Library used for parsing EXIF:  https://github.com/mayanklahiri/easyexif

* You will also need to install `python3` and `pip3 install folium`

## Compile

* `cmake .` , `make` and `./<filename>` 

## How to use

* Press "Add directory" button.
* Choose directory that will be scanned for *.jpg files.
* Wait until it index all files (it may freeze for a bit)
* Press "Show on map" and wait again. It'll generate .txt file with all gps coordinates in project folder and then it python script will be executed. This will generate two .html files with map (also in project folder). Programm will automaticaly open map.html in your default browser.  

## Program look like this:

![prog](images/image_2020-06-03_14-17-18.png)

## Map example:
* ### Map will look like this:
![example2](images/70820424-b3d6b680-1de9-11ea-9da5-afc15f1b2501.jpg)

* ### Heatmap will look like this:
![example1](images/70820424-b3d6b680-1de9-11ea-9da5-a2fc15f1b2501.jpg)

## To-do: 
* Find out how to properly kill threads (rn you can spam "Show on map button and each time it will create a new thread")
* Learn patterns
