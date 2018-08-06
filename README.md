# MSP430F5529 Proximity alarm

### Descrição:

Um programa que implementa um alarme de proximidade para o microcontrolador MSP430F5529. Trabalho final da disciplina Laboratório de sistemas microprocessados 2018/1 da Universidade de Brasília.

### Integrantes do grupo:

Nome | Matrícula
---  | ---
André Laranjeira | 16/0023777
Luiz Antônio Martins | 16/0013615

### Equipamento utilizado:

* Microcontrolador MSP430F5529
* Módulo bluetooth serial HC-05
* Módulo de buzzer 5V
* Módulo de sensor ultrassônico HC-SR04

### Pinos e alimentação dos módulos:

#### Tabela de pinos dos módulos:

Pino | Sinal
---	 | ---
P1.0 | LED 1 (vermelho)
P1.1 | Botão S2
P1.2 | HC-SR04: Trigger
P2.0 (TA1.1) | HC-SR04: Echo
P2.1 | Botão S1
P3.3 | HC-05: RX
P3.4 | HC-05: TX
P4.7 | LED 2 (verde)
P8.2 | Buzzer: I/O

#### Tabela de alimentação dos módulos:

Módulo | Voltagem
---	   | ---
Módulo de buzzer | 3V3
Módulo HC-05 | 3V3
Módulo HC-SR04 | 5V

### Configuração:

1) Compile os arquivos da pasta src com a utilização dos cabeçalhos da pasta inc.
2) Conecte os módulos auxiliares ao MSP430F5529 conforme as tabelas de pinos e de alimentação.
3) Carrege o MSP430F5529 com o executável gerado.
4) Caso queira receber notificações bluetooth do alarme, conecte um dispositivo via bluetooth ao módulo HC-05.
5) Execute o programa no MSP430F5529.

### Modo de uso:

O alarme de proximidade possui 4 estados:
* Idle: O alarme está desativado. Este é o estado inicial do alarme. Nesse estado, apenas o LED vermelho estará aceso.
* Arming: O alarme está sendo armado. Nesse estado, os LEDs vermelho e verde piscam de forma síncrona.
* Armed: O alarme está armado. Nesse estado, apenas o LED verde estará aceso.
* Triggered: O alarme foi disparado. Nesse estado, os LEDs vermelho e verde piscam de forma alternada. 

##### Armando o alarme:
O alarme pode ser armado por meio do botão S1 sempre que o alarme estiver no estado Idle. Após apertar o botão S1, o alarme entrará no estado Arming, no qual o módulo HC-SR04 será utilizado para tomar 4 medições de distância. Caso alguma das 4 distâncias mensuradas seja muito discrepante da média das 4 distâncias, o alarme retornará ao estado Idle, indicando uma falha de armação do alarme. Caso cada uma das 4 distâncias mensuradas esteja próxima da média das 4 distâncias, o alarme terá sido armado com sucesso, entrando no estado Armed. Ao ser armado, o alarme emitirá uma notificação de bluetooth indicando que foi armado e a média das 4 distâncias será utilizada como a distância de armação para o módulo HC-SR04.

##### Disparando o alarme:
Quando o alarme estiver no estado Armed, o módulo HC-SR04 será utilizado para realizar constantes medições de distância. Caso alguma dessas distâncias mensuradas seja muito discrepante em relação à distância de armação do módulo HC-SR04, o alarme será disparado, entrando no estado Triggered. Ao ser disparado, o alarme emitirá uma notificação de bluetooth indicando que foi disparado, interromperá as medições de distância realizadas pelo módulo HC-SR04 e ativará o módulo de buzzer.

##### Desarmando o alarme:
O alarme pode ser desarmado por meio do botão S2 sempre que o alarme estiver no estado Armed ou Triggered. Após apertar o botão S2, o alarme entrará no estado Idle. Ao ser desarmado, o alarme emitirá uma notificação de bluetooth indicando que foi desarmado, interromperá as medições de distância realizadas pelo módulo HC-SR04 caso estas estejam sendo feitas e desativará o módulo de buzzer caso este esteja ativo.

### Observações gerais:

* A branch INTs, gerenciada pelo @labm1997, contém uma versão otimizada do programa que utiliza interrupções e modos de baixo consumo.
* A biblioteca de funções MSP430_shortcuts foi desenvolvida ao longo do semestre com o intuito de facilitar a programação para o MSP430F5529. Ela inclui atalhos na forma de funções, macros e defines para funcionalidades como UART, I2C, LCD, entre outros.
* Alguns microcontroladores semelhantes ao MSP430F5529 talvez sejam capazes de executar esse programa, mas, para isso, talvez seja necessário alterar o código fonte e a tabela de pinos dos módulos auxiliares.
