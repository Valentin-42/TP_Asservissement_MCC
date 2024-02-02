
<!-- PROJECT LOGO -->

<div align="center">

 <h1 align="center">TP Asservissement MCC</h3>
</div>

<div>


<h3 align="center">Table of content</h3>

# 📖 [Documentation](https://valentin-42.github.io/TP_Asservissement_MCC/)

1. [TP1 ](#TP1)
2. [TP2](#TP2)
3. [TP3](#TP3)
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
  
➡️ $Fclk = 100MHz  \implies Tclk = 10ns$<br/>
 ${2*Tclk= 20 ns} \implies {Fclk = 2e-8 Hz}$<br/>
Dead time souhaité : $2 us \implies Fdt = 2e-6 Hz$
<br/>
➡️ ${Fdt\over Fclk}= {2e-6\over2e-8} = 100$
<br/>
📖 Via doc (ref manuel cartes G4) formule 2 : $Tdt =  (64 + **36**)*2t = 2us$ avec bits[7:5] = 10.
<br/>
✏️ On écrit donc 
$10100100 = 0xA4 = 164$ dans le registre TIMx_BTR (ou champ dead time dans STM32CubeMx)

***

🔴 Génération PWM complémentaire décalée de T/2 :  🔴<br/>
 Tim counter : center aligned<br/>
PWM 1 : 80% rapport cyclique / PWM 2 : 20% du rapport cyclique <br/>
Fréquence PWM autour de 16 KHz => {FCLK : 100Mhz / PSC : 5 / ARR : 12500} <br/>

***
🔌Branchements :

Alim CC 42V 
DC+ => Red phase Top/Bott     => PWM1 / PWM1 complémentaire <br />

DC- => Yellow phase Top/Bott => PWM2 / PWM2 complémentaire <br />
Ne pas alimenter/gnd le PCB <br />
Set speed to 50% => moteur à l'arret<br />

***

- Test du moteur en boucle ouverte ✅
Remarques : <br/>
a) Mise en défaut du hacheur lorsque les commandes de vitesses sont trop espacé. (ex speed = 50% puis speed = 70%) Cause : Pic de courant car commande trop brusque => Besoin d'asservissement en courant. <br /> 
b) Saut de tension lors du freinage car alimentation non réversible. <br/>

- Mesure du courant moteur  ⚙️ A faire

- Lecture de l'encodeur  ⚙️ A faire


## TP3

-  〽️ Mesure du courant moteur :

Relevé du courant par les pins du hacheur à l'oscilloscope :
<br />
    <img src="https://github.com/Valentin-42/TP_Asservissement_MCC/blob/main/1.png" alt="1- Speed = 60" width="800" height="480">
 <br />
    <img src="https://github.com/Valentin-42/TP_Asservissement_MCC/blob/main/2.png" alt="2- Speed = 80" width="800" height="480">
 <br />

La fréquence du signal est de $Fsig = 1.5 kHz$ <br />
On échantillonnera à une fréquence au moins deux fois supérieure. (Shannon)
***
- Configuration de l'ADC : <br />


➡️D'après la doc :  ${Fsampling  }= {Fclk  \over sampling-time[cycles] + 0.5 + résolution[bits]}$<br />
On a pris 

➡️ Résolution 12 bits = 4096 max  <br />
➡️ Chanel 1 sur phase Jaune  <br />

On génère une interruption sur conversion de l'ADC.  <br />

➡️ Scaling Hall Current sensor $12 A/ V$ & $2.5 V = 0 A$<br />
➡️La tension max d'entrée de l'ADC est de 3.3V <br />
On remonte à la valeur de courant via :
${I}={((Vadc *3.3)-2.5 )*12 \over 4096}$  <br />


***
- 🏃 Mesure de vitesse :


➡️ Configuration d'un timer en encodeur (combine channel encodeur mode). 



***

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
