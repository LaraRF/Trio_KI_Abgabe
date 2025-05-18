Trio Spiel

Anleitung:
Bei Trio wird ein Zahlenfeld gegeben, das aus den Ziffern 1-9 besteht. In jeder Runde gibt es eine Zielzahl.
Das Ziel ist es, eine Kombination aus 3 Zahlen zu finden, die diese Zielzahl ergeben. 
Dafür gelten folgende Regeln:
1. Die gewählten Zahlen müssen in einer Reihe sein. D.h. 3 nebeneinander horizontal, vertikal oder diagonal liegende Zahlen.
    Es dürfen keine Lücken zwischen den Zahlen sein. Die Zahlen dürfen nicht "um die Ecke" liegen und der Spielfeldrand ist das Ende (das ist ungültig: zwei Zahlen -> Rand -> eine Zahl am gegenüberliegenden Ende, quasi "umgeklappt")
2. Die ersten beiden gewählten Zahlen werden miteinander multipliziert. Die 3. Zahl wird entweder addiert oder subtrahiert.

Es kann zwischen unterschiedlichen Feld-Größen gewählt werden (5x5, 7x7 oder 10x10).
In den Einstellungen kann die Rundenzahl festgelegt werden (Minimum: 1, Maximum: 50).

Zusätzlich kann dort entschieden werden, ob der KI-Assistent verwendet werden soll.
Während dem Spiel gibt es ein "Hinweis"-Feld. Wird dieses angeklickt, wird eine Zahl markiert, die mit zwei weiteren Zahlen eine gültige Lösung ergeben kann.
(Hinweis: Manchmal sind mehrere Lösungen möglich. Es wird aber durch die Verwendung von KI sichergestellt, dass es immer mindestens eine gültige Lösung gibt)
Wird der KI-Assistent aktiviert, können zusätzliche Hinweise gegeben werden, die sich in Textform am unteren Rand des Bildschirms zeigen.
Diese sind abhängig von der Zielzahl, sowie von evtl. bereits markierten Feldern. 
Das Spiel zählt die verwendeten Hinweise mit.
Falls gegen die KI gespielt wird, ist dies ein Faktor in der Bestimmung des Schwierigkeitsgrades.

In den Einstellungen kann zusätzlich gewählt werden, ob man alleine oder gegen einen KI-Gegner spielen möchte.
Wenn man alleine spielt, gibt es keinen Zeitdruck. Die Gesamt- und Durchschnittszeit wird angezeigt, hat aber keine Auswirkung auf das Spiel.
Wird gegen die KI gespielt, können 3 Schwierigkeitsgrade gewählt werden: einfach, mittel und schwer.
Je nach Schwierigkeitsgrad entscheidet sich die Schnelligkeit, mit der die KI spielt.
Im Verlauf des Spiels hinweg analysiert die KI die Geschwindigkeit des Spielers/der Spielerin und passt sich entsprechend an, um den gewünschten Schwierigkeitsgrad bezubehalten und nicht zu herausfordernd/unterfordernd zu sein.
Die Faktoren, nach denen sich die KI anpasst, sind folgende: Durchschnittszeit, verwendete Hinweise, Fehler

Beim einfachen Modus ist die KI etwas langsamer als die Durchschnittszeit des Spielers/der Spielerin (ca. 30%).
Bei mittleren Modus passt sich die KI an die Durchschnittszeit an. 
Beim schweren Modus ist die KI etwas schneller als die Durchschnittszeit (ca. 30%)



Es werden hauptsächlich folgende KI-Komponenten verwendet:
1. Tree-Search-Algorithmus (DFS)
2. Neural Network (FFN)


Das Projekt basiert auf dem 1989 (meine Ausgabe ist von 1994, Nr. 003587) im Ravensburger Verlag (Mitbring-Spiele Reihe) erschienenen "Trio" von Heinz Wittenberg.
Dort wird es für 1-6 Kinder in der Altersklasse von 7-12 Jahren empfohlen.

Das Ursprungsspiel wird auf einem 7x7 Feld mit folgender Zahlenverteilung gespielt. Die Verteilung wurde bei dieser digitalen Version übernommen und für die anderen Feldgrößen entsprechend angepasst.
1: 5 mal
2: 6 mal
3: 6 mal
4: 6 mal
5: 6 mal
6: 6 mal
7: 5 mal
8: 5 mal
9: 4 mal

Die Zielzahlen befinden sich zwischen 1 und 50.


Das Projekt wurde unter Verwendung von Claude.AI, sowie dem analogen Ursprungsspiel und Vorlesungsmaterialien und -notizen erstellt.



```
                | (_) |       | |           | |           
 _ __ __ _ _   _| |_| |__  ___| |_ __ _ _ __| |_ ___ _ __ 
| '__/ _` | | | | | | '_ \/ __| __/ _` | '__| __/ _ \ '__|
| | | (_| | |_| | | | |_) \__ \ || (_| | |  | ||  __/ |   
|_|  \__,_|\__, |_|_|_.__/|___/\__\__,_|_|   \__\___|_|   
            __/ |                                         
           |___/  
```

# raylibstarter - minimal letterbox edition

[![CMakeBuilds](https://github.com/chfhhd/raylibstarter-minimal/actions/workflows/cmake.yml/badge.svg)](https://github.com/chfhhd/raylibstarter/actions/workflows/cmake.yml)

A simple raylib project template for CMake and C/C++ including letterbox rendering and a fullscreen mode.

## Usage

Use CMake or a CMake compatible development environment to build a minimalistic raylib project. The raylib library will be downloaded automatically by CMake.

### Changing the project title and version number

The project name and version number can be customized in the `src/CMakeLists.txt` file. By default, the project name is 'game':

```
project(game VERSION 0.1 LANGUAGES CXX)
```

### Change the window size and switch to fullscreen

The initial width and height of the output window can be adjusted in the `src/config.h.in` file.

By removing the comment

```
//#define GAME_START_FULLSCREEN
```

the project starts in full screen mode.

While running the user can change the window size with the OS's regular means and switch to fullscreen with left alt + enter.

### Manage assets

Store assets in the designated 'assets' folder. The project already contains a sample graphic file. In the `main.cpp` file, this image is loaded and displayed.

### Choose a different raylib version

Which raylib version is used can be specified in the `cmake/raylib.cmake` file. If this is changed after CMake has already created the project once, you must use CMake to completely rebuild the project.

### What next?

Modify the `main.cpp` file according to your needs.

### Create a binary distribution

The cpack command can be used on the command line to create a binary distribution of the project, for example:

```
cpack -G ZIP -C Debug
```

All assets will be packed into the distribution.

## License

see `LICENCE` file for details.
