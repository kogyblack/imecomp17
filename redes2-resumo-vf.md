# Resumo de Redes - VF

## 01 - Camada de Rede

### Roteamento

- Direto: origem e destino na mesma rede.
- Indireto: origem e destino em redes diferentes.

##### Algoritmos

- Não adaptativos ou estáticos: shortest path, flooding, flow-based.
- Adaptativos ou dinâmicos: distance vector, link state.

### Open Shortest Path First (OSPF)

##### Características

- Algoritmo de estado de enlace
- É aberto
- Realiza roteamento de acordo com o tipo de serviço
- Faz balanceamento de carga em vez de usar apenas o menor caminho
- Suporte para rotas classless
- Utiliza multicast
- Pode utilizar hierarquia
- Segurança
- Topologia de rede virtual

##### Vantagens em relação ao RIP

- Segurança: mensagens podem ser autenticadas e criptografadas
- Múltiplos caminhos de mesmo custo permitido
- Roteamento de acordo com o tipo de serviço
- Balanceamento de carga
- Suporte a unicast e multicast
- Pode utilizar hierarquia

##### Funcionamento

- Utiliza-se IP diretamente
- É construído um grafo do AS:
  - vértices: roteadores/redes/estações
  - arestas: conexão entre roteadores ou entre roteadores e redes (pode ser
      bidirecional com custos diferentes).
  - informações sobre vizinhos são divulgadas para todos os demais roteadores
      do AS.
- Custo
  - Associado a cada saída de um roteador
  - Adimensionais (baseados em throughput/round-trip time/confiabilidade etc)
  - Custos para redes em outros AS são obtidos de um EGP
- Utiliza Dijkstra

##### Hierarquia OSPF

- SAs muito grandes -> divisão em áreas
- Área 0 -> backbone (todas as áreas são conectadas ao backbone)
- 3 tipos de rotas: entre áreas, na mesma área e entre SAs
- Dois níveis: área local, backbone.
  - Anúncio de estado do enlace **somente na área**
  - Cada nó tem topologia da área detalhada e somente direção conhecida (caminho
      mais curto) para redes em outras áreas
- Roteadores de borda
- Roteadores de backbone
- Roteadores de fronteira: conectam-se a outros ASs

## 02 - Camada de Enlace

- Endereçamento físico
- Controle de acesso ao meio
- Controle de erros e de fluxo

##### Enquadramento

- Contagem de caracteres: qnt de caracteres no quadro
- Enquadramento por caractere: byte de flag no inicio e fim
- Enquadramento por bit: padrão de bits no início e fim

##### Detecção e correção de erros

- Distância Hamming:
  - Detectar: d+1
  - Corrigir: 2d+1
- Bit de paridade: detecta erro simples
- Cyclic Redundancy Check (CRC): detecção
  - T(x) = M(x) * x^r - R(x)
  - T(x) é múltiplo de G(x)
  - Receptor divide T(x) por G(x), se tiver resto houve erro na transmissão
  - **Problema**: se o erro E(x) for múltiplo de G(x)

##### Protocolos Simplex

- Sem restrição: supõe que não há erro ou perda de quadro.
- Stop-and-wait: supõe que não há erro, porém o receptor possui limitações para
    aceitar todo os quadros que chegam
  - uso do ACK
  - Timeout
  - Identificação dos ACK (ACK1, ACK2, etc) -> ver Stop-and-wait ARQ
- Stop-and-wait ARQ
  - ACK0 e ACK1
  - **Somente** envia o próximo quadro após receber a confirmação do anterior
  - No caso bidirecional:
    - Usar 2 SnW, um para cada sentido
    - Técnica de piggybacking: confirmação e dados no mesmo quadro

##### Protocolos de janela deslizante

- Janela deslizante de um bit
  - Utiliza stop-and-wait
  - ACK0 e ACK1
- Go-Back-n ARQ
  - Janela de recepção: tamanho 1
  - Janela de transmissão: tamanho n
  - Sequência de identificação: 0, 1, ..., n
- Selective Repeat ARQ
  - Janela de recepção: tamanho n
  - Janela de transmissão: tamanho n
  - Sequência de identificação: 0,1, ..., 2n-1
  - Receptor armazena os pacotes recebidos sem erro (mesmo fora de ordem)
  - Retransmissão seletiva

## 03 - Protocolos de Acesso ao Meio

### Alocação Estática

- FDMA e TDMA
- Canal é alocado para o usuário de forma independente da sua atividade
- Desperdício  de recursos caso usuário não tenha nada a transmitir
- Número limitado de usuários

### Acesso múltiplo

- ALOHA
  - Transmite sempre que houver dados
  - Frame check sequence (FCS)
    - Frame correto -> envia ACK
    - ACK indica a ocorrência ou não de colisão
  - Utilização máxima do canal: 18%
- Slotted ALOHA
  - Divisão de tempos em intervalos discretos
  - Utilização máxima do canal: 37%
- CSMA
  - Antes de transmitir o terminal "escuta" o meio.
  - np-CSMA (não persistente)
    - Canal livre: transmite
    - Canal ocupado: aguarda um tempo aleatório antes de "escutar" novamente
  - 1-persistent
    - Canal livre: transmite
    - Canal ocupado: espera até ficar livre (mais de uma escutando = colisão)
  - p-CSMA (p-persistente)
    - Canal divido em slots
    - Canal livre: transmite com probabilidade p
    - Canal ocupado: espera até o próximo slot e repete o algoritmo
- CSMA/CD (collision detection)
  - Ocupa canal
  - A transmissão é interrompida imediatamente após o transmissor perceber que
      houve colisão
  - Tamanho mínimo do quadro: 2 * (taxa de transmissão) * (tempo de propagação)

## 04 - Protocolos de Enlace de Dados

- HDLC
  - Utiliza _piggybacking_, _sliding window_ e _bit stuffing_
  - Orientado a bit
  - Contém verificação CRC-16 ou CRC-32
- PPP
  - Detecção de erros
  - Suporta vários protocolos de rede
  - Permite autenticação (PAP ou CHAP)
  - Orientado a caractere

## 05 - IEEE 802 para Redes Locais

### Ethernet clássico

- Topologia LAN em barramento
- 10 Mbps
- CSMA/CD
- Uso de quadros com cabeçalho

### Padrão IEEE 802.3

- Velocidade de transmissão de 10Mbps
- Sinalização Manchester
- Controle de acesso ao meio: CSMA/CD 1-persistente
- Topologia: barra ou estrela
- Tratamento de colisão: BEB

vs Ethernet:
- Preâmbulo
- Interpretação do campo de controle: ethernet -> tipo, 802.3 -> tamanho

##### Opções de meios a 10Mbps

`<taxa de dados> <método de sinalização> <comprimento máximo>`

\#caguei

##### Quadros Ethernet e IEEE 802.3

- Ethernet
  - Preâmbulo: 8 bytes
  - End. Destino: 6 bytes
  - End. Origem: 6 bytes
  - Tipo: 2 bytes
  - Dados: 46 a 1500 bytes
  - Checksum: 4 bytes

- IEEE 802.3
  - Preâmbulo: 7 bytes
  - Start of Frame (SoF): 1 byte
  - End. Destino: 6 bytes
  - End. Origem: 6 bytes
  - Tamanho: 2 bytes
  - Dados: 46 a 1500 bytes
  - Checksum: 4 bytes

Preâmbulo: todos os bytes -> 10101010
SoF: 10101011
Tamanho:
- `<= 1500`: tamanho da área de dados
- `> 1500`: tipo da PDU

A área de dados deve conter entre 46 a 1500 bytes obrigatoriamente. Pode-se
inserir bytes extras sem informação (_padding_) para completar o tamanho mínimo
do quadro.

##### Endereço MAC

- 6 bytes (48 bits)
- 3 octetos -> fabricante, 3 octetos -> terminal
- 3 tipos de endereço: unicast, multicast (01:xx:xx:xx:xx:xx) e broadcast
    (FF:FF:FF:FF:FF:FF)

##### Tratamento de colisões - Binary Exponential Back-off (BEB)

- Havendo colisão as estações envolvidas produzem JAM
- Após o JAM casa estação aguarda um intervalo de tempo aleatório definodo como:
  - Algoritmo (BEB):
    - 1ª colisão: cada estação espera 0 ou 1 slots de tempo
    - 2ª colisão: cada estação espera 0, 1, 2 ou 3 slots de tempo
    - nª colisão: 0 a 2^n-1 slots (limitado a 1024 slots no total)
    - se n = 16 é reportado um erro

### Repetidores e Hubs

- Repetidor: não reconhece o conteúdo da informação (nível físico).
  - Função: amplificar, limpar e retransmitir o sinal
  - Uso: aumentar a extensão da rede
- Hub: "repetidor de várias portas"
  - Função: concentrar o tráfego de rede, unindo os _hosts_ da LAN
  - Os sinais recebidos em qualquer porta são retransmitidos para todas as
      outras
  - Fisicamente -> estrela, logicamente -> barramento. Portanto tem um único
      domínio de colisão

### Pontes e switches

- Pontes
  - Operam na camada de enlace
  - Dividem a rede em segmentos para reduzir as colisões
  - Redirecionam e descartam quadros de acordo com seus endereços físicos (MAC)
      (origem e destino no mesmo segmento não é repassado)
  - Monta mapa com todos os nós de cada segmento
  - Podem ser usados para conversão de protocolos: 802.x -> 802.y
  - Vantagens:
    - Confiabilidade: cria unidades autocontidas
    - Desempenho: número menor de disputas
    - Segurança: nem todos os dados são enviados para todos os usuários
    - Geografia: permite enlaces de maior distância
- Switches
  - _Tipicamente_ atua na camada de enlace
  - Permite a segmentação da rede sem agregar latência (como nas pontes e
      roteadores)
  - O switch cria um circuito dedicado entre dois segmentos quando se precisa
      passar um quadro de um segmento para outro.
  - Vantagens:
    - Cada dispositivo tem uma capacidade dedicada equivalente à LAN inteira
    - Fácil expansão da rede
  - Tipos:
    - Store-and-forward
    - Cut-through

Diferenças:
- Tratamento do quadro:
  - Ponte: software
  - Switch: hardware
- Pontes -> um quadro por vez, switches -> vários quadros de uma vez
- Pontos -> store-and-forward, switches -> cut-through

### Spanning Tree

- Resolve problemas de loop em redes comutadas cuja topologia introduz anéis nas
    ligações
- Calcula menor caminho, colocando cada porta de bridge/switch como forwarding
    ou blocking

### VLAN

- Agrupamento lógico de dispositivos (independente da localização)
- Isolamento de tráfego
- Inclusão dinâmica
  - Vantagens
    - Segurança
    - Projetos/Aplicativos especiais
    - Desempenho/Largura de banda
    - Broadcast/Fluxo de tráfego
    - Departamentos/Tipos específicos de cargos

### IEEE 802.5 - Token Ring

- Topologia anel
- Uma ou mais permissões (token) circulam pelo anel
- Não existem colisões
- Possui limite máximo para o retardo e tamanho mínimo do anel (deve caber o
    token - 24 bits)
- Acesso ao meio é justo
- Boa estabilidade em alta carga e pouco eficiente em baixa carga
- Estação de origem é responsável pela retirada do quadro da rede (fácil
    confirmação)
- Manutenção do anel: estação monitora
- (Wire center: topologia física estrela, porém implementa anel)

##### Modos de operação

- Antes de transmitir é setado o bit de permissão (permissão ocupada)
- 3 Modos de operação:
  - Single packet: só há uma permissão e, no máximo, um quadro na rede por vez
      (e o quadro deve ser removido por completo antes de liberar o token)
  - Single token: O token é liberado assim que recebido de volta (e só depois
      retira o quadro)
  - Multiple token

## 06 - IEEE 802 para Redes Sem Fio

### IEEE 802.11 - WiFi

- Arquitetura
  - BSS (Basic Service Set): grupo de estações comunicando-se por radiodifusão
      ou infravermelho em uma BSA
  - BSA (Basic Service Area): área de cobertura de um ponto de acesso (AP)
  - Ponto de Acesso (AP): estações responsáveis pela captura das transmissões
      destinadas a estações localizadas em outras BSA
  - Sistema de distribuição (DS): infraestrutura de comunicação que interliga
      múltiplas BSA
  - BSSID (Basic Service Set Identification): identificação única do BSS
  - SSID (Server Set identification): nome _human-readable_
  - ESA (Extended Service Area): interligação de vários BSA pelo DS através dos
      APs
  - ESS (Extended Service Set): (tudo!)
- Topologias
  - AdHoc
  - Infra-estruturado

##### Camada Física

- Dividida em duas subcamadas
  - PLCP: Auxilia a sincronização
  - PMD: transmite os bits da PLCP
- 802.11n
  - MIMO
    - Usa diversas antenas
    - São transmitidos múltiplos fluxos ao mesmo tempo e no mesmo canal

##### Camada de Enlace

- Medium Access Control (MAC): DCF, PCF
  - PCF
    - _Polling_
    - Pouco usado
  - DCF
    - CSMA/CA
  -  Detecção do meio: portadora física ou portadora virtual
- CSMA/CA
  - BEB
  - Janela de contenção: quadro anterior + DIFS + (slots do BEB)
    - CW = ((CWmin + 1) * 2^(i-1)) - 1
  - Interframe Space (do menor pro maior, de maior pra menor prioridade)
    - SIFS: short (antes de CTS/Dados/ACK)
    - PIFS: PCF
    - DIFS: DCF (antes de RTS)
    - EIFS: Extended
  - ACK após todo quadro unicast
  - RTS/CTS
    - Só deve ser usada: em ambientes com grande concorrência ou para envio de
        quadro grande
  - Tipos de mensagem
    - 00: Gerenciamento -> RTS/CTS/ACK/PS-Poll
    - 01: Controle
    - 10: Dados
  - Quadros de gerenciamento:
    - Beacon
    - Probe request/response
    - Association/reassociation request
    - Authentication
    - Disassociation/Deauthentication

## 07 - Padrões WAN

- Tipos de conexão
  - Linha privada
  - Comutado por circuito
  - Comutado por pacote
- Frame Relay
  - Não usa controle de erros
  - Controle de fluxo é fim a fim
  - DTE/DCE
  - DLCI: identifica o circuito virtual dentro da nuvem Frame Relay (dinâmico
      com IARP ou manual)
  - LMI: padrão de sinalização entre um roteador cliente e um switch frame-relay
    - Responsável pelo gerenciamento e manutenção da comunicação
  - CIR: taxa contratada
  - bit DE: 0 se tiver que entregar, 1 se ultrapassar o CIR podendo não ser
      entregue
- ATM
  - Transporte fim a fim integrado por voz, vídeo e dados
  - Orientado à conexão
  - Pacote percorre a rede e configura a conexão
  - Usa células (53 bytes)
    - Propriedades da comutação a pacote e à circuito
    - Células pequenas são boas para tráfego de voz
- MPLS
  - Insere 20 bits de rótulo
  - Encaminha pacotes pelos roteadores (LSR) sem consultar a tabela tradicional
  - LSR: encaminha baseado no rótulo
  - LSP: caminho dentro da rede
  - FEC: grupo de pacotes roteados por um mesmo caminho
  - LFIB: tabelas de encaminhamento

## 08 - Camada Física

- Dispersão modal: raios incidentes no cladding com ângulos diferentes, causando
    espalhamento. Gera atenuação do sinal, portanto é necessário reduzir o
    tamanho do cabo utilizado.

Tipos de fibra:
- Multimodo índice degrau
- Multimodo índice gradual
- Monomodo

Cabeamento estruturado:
- Distribuidor de campus
- Backbone de campus
- Distribuidor de edifício
- Backbone de edifício
- Distribuidor de piso
- Cabeamento horizontal
- Ponto de consolidação
- Cabo do ponto de consolidação
- Tomada de telecomunicações

Tipos de Conexão:
- Cruzada
- Interconexão
