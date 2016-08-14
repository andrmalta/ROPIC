# ROPIC

##Controle do servo-motor
O servo-motor é alimentado com tensões de **5v** e recebe o sinal no formato PWM (Pulse Width Modulation), variando de 0v ou 5v. Para controle é necessário realizar um pulso a cada **20ms**, ou seja, frequência de 50Hz. O comprimento do pulso determina a distância que o motor gira, um sinal com largura de pulso de 1ms corresponde a posição do servo todo a esquerda ou a 0 grau; um sinal com largura de pulso de 1,5ms corresponde a posição central do servo ou de 90 graus; um sinal com largura de pulso de 2ms corresponde a posição do servo todo a direita ou 180 graus. Podendo ser usado qualquer valor intermediário entre 1ms e 2ms para posições proporcionalmente intermediarias.

##Servo-motor com **PIC18F4550**
Para a realização do controle dos servo-motores o PIC18F4550 foi utilizado, juntamente com um Crystal externo, que pode ser de **16MHz ou 20MHz**, para cada um é carregado a configuração para base de cálculos de acordo com a biblioteca desenvolvida, visando obter melhora na precisão.   Contudo, para melhor exemplificar a lógica aplicada para o funcionamento do controle dos servos-motores, vamos utilizar o clock de **16Mhz**. 

O **PIC18F4550** consta com vários módulos de Timer, sendo utilizado para o projeto o Timer 1 configurado com 16bits e prescaler 1:4. 
Como descrito no tópico anterior, o servo-motor possui um período completo de **20ms**, então para utilizar até **8 servos**, de maneira **multiplexada**, precisamos de um tempo de **20/8 = 2,5ms**, este tempo é o subperíodo definido para cada um dos servo-motores. Dentro desse tempo, é realizado o controle de cada servo, um após o outro. Podemos reparar isso na imagem abaixo, que mostra um exemplo de controle multiplexado dos servos.

Esta configuração no Timer 1 gera um pulso de contagem de **1us**, contudo se, queremos um tempo de 1,5ms (nível alto) simplesmente faremos o timer1 contar até 1500, ou seja, carregar o timer1 com 65535-1500 = 64035. O tempo restante do subperíodo definido para o servo-motor de 2,5ms deve ser em nível baixo, então devemos carregar novamente o timer1 com 65535-1000 = 64535, gerando um estouro de 1ms para completar o subperíodo.

Basicamente é definido o nível logico alto para o pino, carregado o timer 1 com valor respectivo a quantidade de graus que se deseja movimentar (entre 1000 e 2000, como visto no tópico anterior) e espera o estouro do timer 1. Logo é definido o nível logico baixo para o pino, carregado então o timer 1 com o tempo restante do subperíodo, espera o estouro e repete o ciclo para um outro servo. Os cálculos para um clock de 20MHz são os mesmos, somente é realizado em outra escala, a mudança será no pulso que passa a ser de 0.8us, diferentemente do exemplificado que é de 1us.

##Biblioteca SERVO.H
A biblioteca “servo.h” foi desenvolvida no intuito de facilitar o controle de cada servo, de maneira multiplexada, seguindo a lógica de interrupções do Timer1 descrita e exemplificada no tópico anterior, segue abaixo suas especificações, restrições e descrições de suas funcionalidades:
###Funções

* **_SetClockBase:_**	Seleciona o clock para realização da base de cálculos do Timer1, somente é possível configura-lo para 16MHz ou 20MHz. Como padrão clock de 20MHZ.

* **_SetPulseWidth:_**	Define a largura do pulso utilizado pelo servo-motor. Obs: cada fabricante possui suas especificações.

* **_SetServoScale:_**	Configura a posição mínima e máxima dos servo-motores. Obs: somente permitido valores maiores que 0.

* **_InitServos:_**   	Prepara para uso os servo-motores e configura a porta de saída do pulso, sendo _PORTB_ ou _PORTD_.

* **_MoveServoAsync:_**	Movimenta o servo-motor desejado para a posição informada de forma assíncrona.

* **_MoveAllServos:_**	Movimenta todos os servo-motores de maneira assíncrona com a mesma posição informada.

* **_GetServoPosition:_** Retorna a posição atual do servo-motor.

* **_GetMinPosition:_**	Retorna a posição mínima.

* **_GetMaxPosition:_**	Retorna a posição máxima.

[Descrição Completa da Biblioteca SERVO.H](https://github.com/andrmalta/ROPIC/blob/master/ROPIC.pdf)

