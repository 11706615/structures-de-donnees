# Projet : Réseau d’Aqueducs
## Compilation du programme
L'ensemble du programme est écrit en C++17. Le programme peut être compilé en 64 bits ce qui permet une utilisation moins limitée de la mémoire.
### Visual Studio 2019
Le git contient un projet configuré, il suffit d'ouvrir la solution .sln.
### g++
Un makefile est fourni, il suffit de se déplacer dans le dossier "Réseau d’Aqueducs" puis taper `make` dans un terminal. Le Makefile générera un fichier "prog" qui pourra être éxécuté de la manière suivante :
```bash
./prog citiesList.csv resuCities.dat resuGraph.dat
```
## Exécution du programme
Une fois compilé, le programme doit être éxécuté en passant les arguments `ImporterVilles [ExporterVilles] [ExporterArretes]` avec :
- `ImporterVilles` Emplacement du fichier contenant les villes à importer.
- `ExporterVilles` (Facultatif) Emplacement du fichier qui contiendra les villes dont la population correspond.
- `ExporterArretes` (Facultatif) Emplacement du fichier qui contiendra les arretes du graphe minimal correspondant.
## Utilisation du programme
Une fois le programme lancé, il faut taper le nombre d'habitants minimum. Toute entrée qui ne soit pas un nombre provoquera l'arrêt du programme. Après saisie, le nombre de villes et la longueur du réseau sont affichés.
