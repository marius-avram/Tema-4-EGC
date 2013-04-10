AVRAM Marius 334CB
Tema 4 EGC - SpaceEscape 2013

Toate cerintele din enunt au fost indeplinite. Am incarcat o nava cu un aspect 
cat mai futuristic dintr-un fisier off, setandu-i in acelasi timp normalele 
si proprietatile de material. Aceasta se poate misca doar in limitele unui 
paralelipiped destul de mare comparativ cu dimensiunile navei. Pe fetele
paralelipipedului am generat puncte aleator pentru a reda efectul de stele de 
pe fundal. Am ales aceasta varianta in locul unui skybox din poze pentru a ma 
limita la notiunile invatate la laborator. 

Asteroizii se genereaza in planul din fata cu diferite caracteristici (dimensiune,
culoare, viteze, etc) si se translateaza la fiecare cadru spre planul din spate 
al cutiei virtuale. De fiecare data cand depaseste acest plan un asteroid este 
sters si in locul lui este generat altul. De asemenea si in cazul in care un 
asteroid este distrus cu razele laser altul este generat in planul din fata.
Astfel numarul asteroizilor ramane constant.

Razele laser sunt reprezentate prin niste linii punctate care se translateaza 
de la nava la asteroid pentru a da efectul de miscare. Atunci cand un asteroid
este distrus acesta isi schimba culoarea (temporar), este impartit in mai multe 
bucati de asteroid care se departeaza una fata de cealalta si isi scad din 
transparenta pana devin invizibile. Cand bucatile sunt complet invizibile 
asteroidul respectiv este sters din scena.

Am folosit 2 surse de lumina omnidirectionale si 2 spotlighturi.
Am implementat toate camerele cerute in enunt + camera TPS pentru ca mi se pare 
ca ofera o experienta de joc mult mai buna.

Am facut si o interfata pentru scor, insa din anumite motive cand o desenam la 
sfarsit dupa toate elementele pickingul nu mai mergea. Asa ca am desenat-o la 
inceput si cateodata s-ar putea sa fie acoperita de asteroizi, deoarece 
DEPTH_TEST-ul este activat. 

Comenzile sunt urmatoarele: 
- click stanga - distrugere sau selectare camera asteroid
- 1 - camera din cabina
- 2 - camera TPS
- 3 - mod camera asteroid (in care trebuie selectat cu click stanga asteroidul 
pe care se doreste plasata camera).
- 4 - camera dinamica
- w,a,s,d - inainte, stanga, inapoi, dreapta
- r,v - sus jos
- f - aprinderea celor doua lumini de tip spotlight/lanternelor
- q,e|t,g|z,c - rotirea pe toate axele a camerei TPS. Doar acesta poate fi
rotita. Celelalte camera nu se pot roti pentru ca nu este solicitat explicit in 
enunt acest lucru.