# README - Sistema de Semáforo e Controle de LEDs com Raspberry Pi Pico W

## Autor
Kalel Ezveither

## Descrição
Este projeto implementa um **sistema de semáforo** e um **controle de LEDs via botão** utilizando a **Raspberry Pi Pico W**. O código foi desenvolvido em **C** com o **Pico SDK**.

## Funcionamento
### Modo Semáforo (Automático)
- Os LEDs operam como um semáforo tradicional: **Vermelho → Amarelo → Verde**.
- Cada mudança de estado ocorre a cada **3 segundos**.
- O status do semáforo é exibido via **UART (porta serial)**.

### Modo Botão (Intervenção Manual)
- O botão (GPIO 5) **pausa o semáforo** e acende todos os LEDs.
- Após pressionar, os LEDs **desligam um a um** a cada **3 segundos**.
- Quando todos os LEDs forem desligados, o semáforo **retoma automaticamente**.
- O botão só pode ser pressionado novamente após a sequência manual terminar.

## Componentes Utilizados
- **Microcontrolador:** Raspberry Pi Pico W
- **LEDs:** RGB (conectados aos GPIOs 11, 12 e 13)
- **Botão:** Pushbutton (conectado ao GPIO 5)
- **Resistores:** 330Ω para cada LED

## Como Rodar o Código
1. **Configurar o ambiente** com o Pico SDK no **VS Code**.
2. **Compilar e enviar** o código para a Raspberry Pi Pico W.
3. **Monitorar a saída** pela UART para visualizar logs do sistema.
4. **Testar no Wokwi** ou na placa real com a ferramenta **BitDogLab**.

## Requisitos Técnicos
- **C com Pico SDK**
- **Plataforma VS Code**
- **Simulação no Wokwi ou hardware real**
- **Controle via UART para depuração**

---

Projeto desenvolvido por **Kalel Ezveither**.

