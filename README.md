# 🛰️ NASA Disaster Monitor — Global Solution 2026

> Sistema de monitoramento global de desastres naturais e aquecimento climático via satélite, desenvolvido com Arduino e simulado no Wokwi.

---

## 📋 Descrição do Projeto

O **NASA Disaster Monitor** é um sistema embarcado de monitoramento ambiental que simula o uso de dados de satélites da NASA (API EONET) e da rede Starlink da SpaceX para acompanhar em tempo real eventos de desastres naturais e anomalias de temperatura ao redor do planeta.

O projeto foi desenvolvido como parte do **Global Solution 2026** da FIAP, com foco no tema **Espaço**, explorando como tecnologias espaciais podem ser aplicadas na proteção da vida humana na Terra.

---

## 🎯 Objetivo da Solução

Demonstrar como dados coletados por satélites em órbita podem ser processados na borda da rede (Edge Computing) para gerar alertas rápidos e precisos sobre desastres naturais, contribuindo para sistemas de defesa civil, prevenção de catástrofes e monitoramento climático global.

A solução integra duas frentes:

- **Monitoramento de desastres** — dados da API EONET da NASA registrando eventos como tufões, vulcões, incêndios, terremotos e enchentes em tempo real
- **Monitoramento climático** — rede Starlink como infraestrutura distribuída de coleta de temperatura global, identificando anomalias térmicas que precedem desastres

---

## 🧩 Componentes Utilizados

| Componente | Quantidade | Função |
|---|---|---|
| Arduino Uno | 1 | Microcontrolador principal |
| LCD 16x2 | 1 | Exibição de eventos e temperatura |
| Sensor DHT22 | 1 | Leitura de temperatura e umidade |
| LED Vermelho | 1 | Alerta de vulcão |
| LED Azul | 1 | Alerta de tempestade/tufão |
| LED Amarelo | 1 | Alerta de incêndio |
| LED Verde | 1 | Alerta de terremoto |
| LED Laranja | 1 | Alerta de enchente |
| LED RGB | 1 | Indicador de nível de risco geral |
| Buzzer | 1 | Alerta sonoro por nível de risco |
| Botão (verde) | 1 | Navegar para o próximo evento |
| Botão (vermelho) | 1 | Navegar para o evento anterior |
| Potenciômetro | 1 | Ajuste de contraste do LCD |
| Resistores 220Ω | 5 | Proteção dos LEDs |

---

## ⚙️ Explicação do Funcionamento

### Boot
Ao iniciar, o sistema exibe uma animação de boot no LCD com as mensagens **"NASA MONITOR"** e **"Starlink Online / EONET API: OK"**, seguida de um piscar de todos os LEDs indicando que o sistema está pronto.

### Exibição de Eventos
O LCD exibe os dados do evento atual em duas linhas:
- **Linha 1:** Nome do evento (ex: `Tufao Mawar`)
- **Linha 2:** Região + Nível de risco (`LOW`, `MED`, `ALT`)

O sistema conta com **10 eventos pré-carregados** simulando dados reais da API EONET da NASA, incluindo tufões, incêndios, vulcões, terremotos e enchentes em diferentes regiões do mundo.

### LEDs por Categoria
Cada categoria de desastre acende um LED específico:
- 🔴 **Vermelho** → Vulcão
- 🔵 **Azul** → Tempestade / Tufão
- 🟡 **Amarelo** → Incêndio
- 🟢 **Verde** → Terremoto
- 🟠 **Laranja** → Enchente

### LED RGB — Nível de Risco
- 🟢 **Verde** → Risco Baixo
- 🟠 **Laranja** → Risco Médio
- 🔴 **Vermelho** → Risco Alto

### Buzzer
O buzzer apita em intervalos proporcionais ao nível de risco:
- Risco Baixo → beep a cada 3 segundos
- Risco Médio → beep a cada 1,5 segundos
- Risco Alto → beep a cada 0,5 segundos

### Temperatura (DHT22)
A cada 10 segundos, o LCD alterna para exibir a leitura atual de temperatura e umidade do sensor DHT22, simulando os dados coletados pela rede Starlink. Se a temperatura ultrapassar **35°C**, o sistema emite um alerta sonoro e o LED RGB acende em vermelho.

### Navegação
Os dois botões permitem navegar entre os eventos cadastrados (próximo / anterior) de forma intuitiva.

---

## 🔌 Estrutura do Circuito

```
Arduino Uno
├── Pino 2-5     → LCD (D4-D7)
├── Pino 6       → DHT22 (DATA)
├── Pino 7       → LED Laranja (Enchente) via resistor 220Ω
├── Pino 8       → LED RGB (R)
├── Pino 9       → LED RGB (G)
├── Pino 10      → LED RGB (B)
├── Pino 11      → LCD (Enable)
├── Pino 12      → LCD (RS)
├── Pino 13      → Buzzer
├── Pino A0      → LED Vermelho (Vulcão) via resistor 220Ω
├── Pino A1      → LED Azul (Tempestade) via resistor 220Ω
├── Pino A2      → LED Amarelo (Incêndio) via resistor 220Ω
├── Pino A3      → LED Verde (Terremoto) via resistor 220Ω
├── Pino A4      → Botão PRÓXIMO (INPUT_PULLUP)
├── Pino A5      → Botão ANTERIOR (INPUT_PULLUP)
└── 5V / GND     → LCD, DHT22, LEDs, Potenciômetro
```

---

## 🚀 Instruções de Execução

### No Simulador Wokwi

1. Acesse o link da simulação: **[inserir link do Wokwi aqui]**
2. Clique em ▶️ **Start Simulation**
3. Use os botões verdes e vermelhos para navegar entre os eventos
4. Aguarde 10 segundos para ver a leitura de temperatura do DHT22

### Localmente (Arduino físico)

1. Clone este repositório:
```bash
git clone https://github.com/seu-usuario/nasa-disaster-monitor.git
```
2. Abra o arquivo `sketch.ino` na Arduino IDE
3. Instale as bibliotecas necessárias:
   - `LiquidCrystal` (nativa)
   - `DHT sensor library` (Adafruit)
4. Conecte os componentes conforme o diagrama `diagram.json`
5. Faça o upload para o Arduino Uno

---

## 📁 Estrutura do Repositório

```
nasa-disaster-monitor/
├── sketch.ino       # Código-fonte Arduino/C++
├── diagram.json     # Diagrama do circuito (Wokwi)
├── libraries.txt    # Bibliotecas utilizadas
└── README.md        # Documentação do projeto
```

---

## 👥 Integrantes do Grupo

**Liga Dos Devs** — FIAP 2026

| Nome | RM |
|---|---|
| Felipe Romano de Paula Souza | 571653 |
| Lucas Zarantonelli Lourenço | 569164 |
| Nicole Barbosa Oliveira de Lima | 569505 |
| Ryan Romagnoli Santos | 568845 |
| Vinicius Di Tulio Gomes Silva | 573019 |

---

## 🏫 Informações Acadêmicas

- **Instituição:** FIAP
- **Disciplina:** Edge Computing & Computer Systems
- **Atividade:** Global Solution 2026
- **Tema:** Espaço

---

*Desenvolvido com 🛰️ pela Liga Dos Devs — FIAP 2026*
