
<!-- PROJECT LOGO -->

<div align="center">

 <h1 align="center">TP Asservissement MCC</h3>
</div>

<div>


<h3 align="center">Table of content</h3>

 
1. [TP1 ](#TP1)
2. [TP2](#TP2)
3. [TP3](#TP3)
4. [TP4](#TP4)
 </center>
</div>


<br/>
<br/>
<br/>


## TP1

- Shell : 															  ✅
- Génération des PWMs complémentaires : ✅
- Calcul du temps mort : 								  ⚙️ En révision
- Signaux de commande :					 ✅ allumage(Uart + Boutton) ✅ speed
## TP2

⚙️ Calcul du temps mort : 
  
➡️ $Fclk = 100MHz  \implies Tclk = 10ns$
 ${2*Tclk= 20 ns} \implies {Fclk = 2e-8 Hz}$
Dead time souhaité : $2 us \implies Fdt = 2e-6 Hz$

➡️ ${Fdt\over Fclk}= {2e-6\over2e-8} = 100$

📖 Via doc (ref manuel cartes G4) formule 2 : $Tdt =  (64 + **36**)*2t = 2us$ avec bits[7:5] = 10.

✏️ On écrit donc 
$10100100 = 0xA4 = 164$ dans le registre TIMx_BTR (ou champ dead time dans STM32CubeMx)

***

🔴 Génération PWM complémentaire décalée de T/2 :  🔴
 Tim counter : center aligned
PWM 1 : 80% rapport cyclique / PWM 2 : 20% du rapport cyclique 
Fréquence PWM autour de 16 KHz => {FCLK : 100Mhz / PSC : 5 / ARR : 12500} 

***
🔌Branchements :

Alim CC 42V 
DC+ => Red phase Top/Bott     => PWM1 / PWM1 complémentaire 
DC- => Yellow phase Top/Bott => PWM2 / PWM2 complémentaire 
Ne pas alimenter/gnd le PCB 
Set speed to 50% => moteur à l'arret

***

- Test du moteur en boucle ouverte ✅
Remarques : 
a) Mise en défaut du hacheur lorsque les commandes de vitesses sont trop espacé. (ex speed = 50% puis speed = 70%) Cause : Pic de courant car commande trop brusque => Besoin d'asservissement en courant.  
b) Saut de tension lors du freinage car alimentation non réversible.

- Mesure du courant moteur 

- Lecture de l'encodeur 


## TP3
## TP4
### Built With

This section should list any major frameworks/libraries used in this project.

* C/C++ 
* STM32Cube IDE
* MCC 
* Hacheur


##  Commands

<b>Get as local repository :</b>
Clone this repository 

  ```sh
  clone https://github.com/Valentin-42/Robot_Trieur
  ```
<br/>

<b>Update your local repository :</b>
Pull in the local repository folder

  ```sh
  git pull
  ```
<br/>

<b>After modifications :</b>
Add your changes 

  ```sh
  git add *
  ```
  
Commit your changes 

  ```sh
  git commit -m "My changes"
  ```
Push to the distant repository (branch=main)

  ```sh
  git push origin main
  ```
