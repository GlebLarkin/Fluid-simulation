# Fluid-simulation

# Симуляция жидкости методом SPH частиц

## Описание проекта
Данный проект представляет собой симуляцию жидкости с использованием метода Smoothed Particle Hydrodynamics (SPH). В рамках проекта будут реализованы основные компоненты алгоритма SPH для моделирования поведения жидкости(эту умную фразу я с чата гпт скопировал). Стоит отметить, что на момент написания readme мы симулируем жидкость в 2д.

Из планов:
1. Реализовать класс Particle (done)
2. Прописать физику взаимодействия частиц с окружающей средой (done)
3. Реализововать взаимодействие частиц с курсором мыши добавить всякие вау эффекты (done)
4. Реализовать класс Pressure_map и Pressure_map_cell, с помощью которых будет реализовано отталкивание частиц друг от друга (done)
5. Переписать все на нормальный кодстайл и разбирать на несколько файлов <---мы сейчас тут
6. Жестко это все оптимизировать
7. Реализовать вязкость жидкости(тут понадобится небольшой research)
Далее по возможности:
1. Добавить пользователю возможность добавлять различные объекты на поле с жидкостью(например, продемостировать работу сообщающихся сосудов)
2. Добавить настройки коэф вязкости/плотности/массы частиц
3. Перейти в 3д(если прям совсем куча времени будет)

//Пока весь код адски нечитаем и написан в мейне, но в ближайшие несколько дней сделаем многофайловую сборку и все станет читаемо

По кодстайлу:
хз) пока не договаривались строго, да и к моменту написания readme написанные строки вообще никакого кодстайла не подразумевают(один из программистов глупенький и не думал об этом, когда писал)

//Далее readme будет дополняться.

English version(using chat gpt):
# Fluid simulation using SPH particles

## Project Description
This project is a simulation of fluid using the Smoothed Particle Hydrodynamics (SPH) method. The main components of the SPH algorithm will be implemented for modeling fluid behavior in this project. It is worth noting that at the time of writing this readme, we are simulating fluid in 2D.

Planned Tasks:
1. Implement the Particle class (done)
2. Define the physics of particle interactions with the environment (done)
3. Implement interactions between particles and the mouse cursor to add visual effects (done)
4. Implement the Pressure_map and Pressure_map_cell classes to handle particle repulsion (done)
5. Refactor the code into a more organized structure and split it into multiple files <— currently working on this
6. Optimize the code rigorously
7. Implement fluid viscosity (may require some research)

Future Tasks:
1. Allow users to add different objects to the fluid field (e.g., demonstrate interacting vessels)
2. Add settings for viscosity/density/particle mass
3. Transition to 3D simulation (if time permits)

//Currently, the code is messy and written in a single file, but in the next few days, we will refactor it into a multi-file structure for better readability.

Regarding Code Style:
Not defined yet, and at the time of writing this readme, there was no specific code style followed in the existing code (one of the programmers didn't consider it while writing).

//The readme will be updated as the project progresses.
