# Fluid-simulation
![Тут должна быть гифка красивая](Short.gif)

[Трейлер](https://youtu.be/HlB1t6TFtqU?si=PZJreQf-DYqVYeH2)
[Описание проекта](https://www.youtube.com/watch?v=X9fE-gI-9j8)


## Как запустить?
### На Linux
1. Для работы проекта необходима библиотека SFML, которую можно установить командой ```sudo apt-get install libsfml-dev```
2. Сборка происходит с помощью утилиты cmake, так что cmake должен быть установлен: ```sudo apt-get install cmake```
3. В терминале переходим в папку (пишем cd <путь к папке>), в которой хотим оставить проект
4. Копируем репозиторий на свой компьютер: ```git clone https://github.com/GlebLarkin/Fluid-simulation.git```
5. Переходим в папку build командой ```cd build```
6. На всякий случай чистим кэш командой ```rm -fr *```
7. Пишем ```cmake ..```
8. Собираем проект командой ```make```
9. Запускаем все с помощью команды ```./MyProj ../examples//config.json```

Если не запускается - проблема при работе с потоками. Проблема решена в этом [видео](https://youtu.be/6gx9xfAC6Wk?si=ONeaTT03Yx5rbeTc), но для запуска симуляции решать ее совсем необязательно - можно просто запустить версию проекта без многопоточки. Чтобы это сделать, нужно в пункте 4 выше склонировать соответствующую ветку командой ```git clone https://github.com/GlebLarkin/Fluid-simulation.git -b Fluid-Simulation-Without-Threads```. До 4 пункта и после него делаем все точно так же. Так частиц будет меньше и производительность будет хуже, но работать все таки будет. Надеюсь))

### На Windows
1. Используйте виртуальную машину с линуксом, пожалуйста
2. См запуск на линуксе

### На Mac
1. Да

Проект на маке мы запускали два месяца. Он работает сильно лучше, но, господа эплбойчики, давайте сами. Мы уже не помним, что мы делали.


## Симуляция жидкости методом SPH

### Что?
SPH - Smoothed particle hydrodynamics - Гидродинамика сглаженных частиц — вычислительный метод для моделирования динамики жидкости и газов (и, кажется, еще и твердых тел, и, наверное, систем тел).
Объясняя болтологически - мы берем тело, делим его на дискретные участки и размазываем их по пространству каким-то известным нам способом. Так можно сильно упростить вычисления внутренних сил, ведь взаимодействием внутри этих самых размазанных участков можно принебречь. Сам метод очень гибкий и позволяет творить какие-то невероятные вещи при правильном его использовании, мы же сегодня остановимся на достаточно базовом (то есть без всяких умных наворотов) его применении - на моделировании жидкости. Матчасть метода очень круто описана у этих [ребят](https://github.com/Shulliikkk/sph-toy-star?tab=readme-ov-file#%D0%BE%D0%B1%D1%89%D0%B5%D0%B5-%D0%BE%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5-sph) - они моделировали процесс образования звезды.

### Как?
Движение вязкой жидкости описывается системой уравнений Навье-Стокса - по сути интерпретация 2-ого закона Ньютона для тел, для которых скорость в точке зависит от координаты (то есть разные части тела движутся с разной скоростью). Вот эти красавчики:

Первое:
$$\rho \frac{du}{dt} = - \nabla p + \mu \nabla ^2 u + \rho F_\text{ext}$$

Второе:
$$\nabla u = 0$$

Тут u - скорость, rho - плотность, p - давление, mu - коэффициент вязкости, F_ext - внешние силы.

Рассмотрим их подробнее. Начнем со второго. Оно выглядит сильно проще (да и на деле оно проще). Оно постулирует сохранение массы - вот и все. Поэтому не будем над ним более размушлять и перейдем к штуке посерьезнее - к первому уравнению.
Это чудо - второй закон Ньютона в непривычной для восприятия форме:

Слагаемое - nabla p отвечает за движение жидкости из области более высокого давления в область более низкого.

mu nabla^2 u - а это - за вязкое трение между слоями жидкости.

Остальные два слагаемые более очевдны - они отвечают за за ускорение тела и за внешние силы соответственно.

Вот очень хороший [видос](https://youtu.be/IE7jMEnzp7g?si=-QTR5m7n6OyR1rhp) на эту тему.

Определить, какие уравнения нужно решать - это, конечно, хорошо. Но как их решать то? Тут как раз и появляется необходимость в SPH - надо дробить жидкость на дискретные части, и для каждой частички писать уже привычный второй закон Ньютона (конечно, держа в голове Навье-Стокса).

### Что делали то?
Важное отступление: мы не стремились к абсолютной физической корректности модели - мы добивались визуального сходства и работы ее в реальном времени.

Из-за страха ударить лицом в грязь и ничего не сделать, мы начнем с того, что оставим идею о трехмерных симуляциях на неопределенный срок и отправимся поить жителей двумерного пространства.
Очевидно, нужны частицы. Сначала пропишем для них базовую физику - гравитацию, отталкивание от стенок и всякие потери энергии из-за трения и сопротивления среды. Отобразим всю это красоту с помощью sfml (как же мы потом будем жалеть об этом. Используйте Unreal engine или что-то подобное, не повторяйте наших ошибок). Потом будем учить частицы оттлакиваться. Есть несколько способов, как это сделать:
  1. Самый очевидный метод - перебрать все частицы, расчитать взаимодействие каждой с каждой за O(n^2). Очевидно, это долго, ведь частиц должно быть много. (P.S. Этот метод можно очень ловко оптимизировать с помощью хэширования координат частичек и сделать его сильно (очень сильно) быстрее. Где-то в интернете есть статья на эту тему)
  2. Второй способ - создать карту давлений. Каждая клетка в карте соответствует перепаду давлений в этой точке, которое определяется количеством частиц рядом и расстояний до них. Соответственно, частицы двигаются в область меньшего давления. Если мы имеем n частиц и m клеток, то ассимптотика будет O(nm), что сильно хуже, чем в первом алгоритме с полным перебором (ведь клеток должно быть больше, чем частиц). К счастью, такой алгоритм легко оптимизируется с учетом того, что можно пренебречь взаимодействием достаточно далеких частиц. Также с помощью изменения размера клеток можно легко контролировать производительность.
  3. Есть крайне мощная модификация второго метода - использование квадродерева. Этот метод, наверное, самый оптимальный, ведь работает за O(n log n), но, к сожалению, мы узнали о нем слишком поздно. Вот [видос](https://youtu.be/OJxEcs0w_kE?si=74fWriVLXfXjsD8C) на эту тему.

Мы выбрали второй метод - он очевиден в плане понимания, проще первого в плане оптимизации. Жаль, что не выбрали третий(

### Немного про силы
Вот функция распределения силы отталкивания от расстояния:
![Тут должен быть график](plot.png)

Важно учесть, что, очевидно, сила давления не должна зависеть от радиуса "размазывания" частицы. Поэтому мы делим ее на нормировочный коэффициент - среднее значение силы. Полученный интеграл вольфрам брать отказался, поэтому мы считаем его численно в питоновском файле. Там же построен тот самый график зависимости силы отталкивания от расстояния.

Также надо реализовать вязкость. Прямо говоря, второй способ вообще не подошел для этих целей, и у нас ничего нормального не вышло. Почему? Потому что силы вязкости, возникающие при движении двух частиц, зависят от их относительной скорости, что, теоретически, можно реализовать, храня в каждой клетке суммарный вектор скоростей всех близлижайших частиц, кроме слишком близких(чтобы частица не взаимодействовала сама с собой), но мы не успели это отладить. Поэтому без вязкости в этот раз. 

### Другое
Чтобы все работало в реальном времени, мы использовали многопоточку.

В какой-то момент мы заметили, что если отображать частицы не резкими кругами, а размытыми, то визуально все сильно лучше - появляется эффект водной глади. Поэтому кружочки размытые. И их цвет зависит от высоты - чем выше, тем зеленее. Так красивше)

Мы не используем STL, тк большая часть проекта была написана до её изучения.

### Что по итогу?
Нам нравится. Будут вопросы - пишите!
