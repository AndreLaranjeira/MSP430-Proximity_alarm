# MSP430 Proximity alarm

### Descrição:

Um programa para microcontroladores da família MSP430 que implementa um alarme
de proximidade. Trabalho final da disciplina Laboratório de sistemas
microprocessados 2018/1 da Universidade de Brasília.

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

##### Tabela de pinos dos módulos:

Pino | Sinal
---	 | ---
P1.0 | LED 1
P1.1 | Botão S2
P1.2 | HC-SR04: Trigger
P2.0 (TA1.1) | HC-SR04: Echo
P2.1 | Botão S1
P3.3 | HC-05: RX
P3.4 | HC-05: TX
P4.7 | LED 2
P8.2 | Buzzer: I/O

##### Tabela de alimentação dos módulos:

Módulo | Voltagem
---	   | ---
Módulo de buzzer | 3V3
Módulo HC-05 | 3V3
Módulo HC-SR04 | 5V

### Configuração:

1) Compile os arquivos da pasta src com a utilização dos cabeçalhos da pasta inc.
2) Conecte os módulos ao MSP430F5529 conforme as tabelas de pinos e de alimentação.
3) Carrege o MSP430F5529 com o programa gerado.
4) Execute o programa no MSP430F5529.

### Modo de uso:

[Em desenvolvimento...]
O botão S1 arma o alarme. O botão S2 desarma o alarme.

### Observações gerais:

* A branch INTs, gerenciada pelo @labm1997, contém uma versão otimizada do programa que utiliza interrupções e modos de baixo consumo.
* A biblioteca de funções MSP430_shortcuts foi desenvolvida ao longo do semestre com o intuito de facilitar a programação para o MSP430.
Ela inclui atalhos na forma de funções, macros e defines para funcionalidades como UART, I2C, LCD, entre outros.
