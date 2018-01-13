#!bin/sh
# What it does:
# Create a new release package and zip it.

#Step 1:Ask the user which version shall be created
clear
echo 'Please enter the version you would like to create (e.g. v1.2)'
read version
echo "Die folgende Version wird erstellt: Labskaus_$version"

##
echo 'Step 2:Erstelle einen neuen Ordner auf dem Desktop mit dem Namen der Programmversion'
Zielordner="$HOME/Desktop/Labskaus_$version"
mkdir $Zielordner

##
echo 'Step 3:Kopiere alle help Dateien in den Zielordner'
cp -R ../help $Zielordner

##
echo 'Step 4:Kopiere das Programm in den Zielordner'
cp ../bin/Release/Labskaus "$Zielordner"

##
echo 'Step 5:Kopiere die zusätzlichen Recourcen in den Zielordner' 
cp -R ../pic $Zielordner

##
echo 'Step 6: Kopiere das configure.sh in den Zielordner'
cp ./configure.sh $Zielordner

##
echo 'Step 7: Das ganze Verzeichnis packen'
cd $Zielordner
cd ..
zip -r "Labskaus_$version".zip "Labskaus_$version"

##
echo 'Fertig'
