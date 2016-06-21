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

##### Alocação Estática

- FDMA e TDMA
- Canal é alocado para o usuário de forma independente da sua atividade
- Desperdício  de recursos caso usuário não tenha nada a transmitir
- Número limitado de usuários

##### Acesso múltiplo

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

##### Ethernet clássico

- Topologia LAN em barramento
- 10 Mbps
- CSMA/CD
- Uso de quadros com cabeçalho

##### Padrão IEEE 802.3

- Velocidade de transmissão de 10Mbps
- Sinalização Manchester
- Controle de acesso ao meio: CSMA/CD 1-persistente
- Topologia: barra ou estrela
- Tratamento de colisão: BEB

vs Ethernet:
- Preâmbulo
- Interpretação do campo de controle (o que seria isso?)

##### Opções de meios a 10Mbps

`<taxa de dados> <método de sinalização> <comprimento máximo>`

\#caguei


