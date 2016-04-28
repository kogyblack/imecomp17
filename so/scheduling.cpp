/**
 *
 * Trabalho sobre Escalonamento
 *
 * Alunos: André Luiz de Mesquita, Narcélio Medeiros, Naum Azeredo
 *
 */

#include <cstdio>
#include <vector>
#include <queue>
using namespace std;

const int rr_timeout = 10;
const int q1_timeout = 100;
const int io_timeout = 20;

struct Process {
  int pid;
  int io, total_burst;
  int burst, timeout;
  int io_start_time;
};

vector<int> gantt_pid;
vector<int> gantt_time;

queue<Process> io;
queue<Process> q0;
vector<Process> q1;
int timer = 0;
int rr = 0;

int main() {
  int total_process_q0,
      total_process_q1;
  printf("Total de processos na fila Q0: ");
  scanf("%d", &total_process_q0);

  for (int i = 0; i < total_process_q0; ++i) {
    printf("Process %d (surto, E/S): ", i);
    int burst, io;
    scanf("%d%d", &burst, &io);

    Process process { i, io, burst, 0, 0, 0 };
    q0.push(process);
  }

  printf("\nTotal de processos na fila Q1: ");
  scanf("%d", &total_process_q1);

  for (int i = 0; i < total_process_q1; ++i) {
    printf("Process %d (surto, E/S): ", i + total_process_q0);
    int burst, io;
    scanf("%d%d", &burst, &io);

    Process process { i + total_process_q0, io, burst, 0, 0, 0 };
    q1.push_back(process);
  }

#ifdef DEBUG
  printf("\n\n");
  printf("Time      : tempo em ms\n");
  printf("Queue     : fila da qual o processo é escalonado (none se nenhum processo foi escalonado)\n");
  printf("pid       : identificador do processo escalonado (none se nenhum processo foi escalonado)\n");
  printf("Q1 timeout: pid dos processos que receberam timeout na fila Q1\n");
  printf("IO queue  : pid do processo utilizado a I/O\n");
  printf("\n\n");

  printf("Time| Queue/pid       |Q1 timeout|IO queue\n");
#endif

  while (!q0.empty() || !q1.empty() || !io.empty()) {
#ifdef DEBUG
    printf("%3d", timer);
#endif

    if (!q0.empty()) { // Se a fila q0 não for vazia, escalona o processo dessa fila
      Process& p = q0.front();
      p.burst++;
      p.timeout++;

#ifdef DEBUG
      printf(" | Q0 %d", p.pid);
#endif

      // Adiciona no diagrama de Gantt
      if (gantt_pid.empty() || gantt_pid.back() != p.pid) {
        gantt_pid.push_back(p.pid);
        gantt_time.push_back(timer);
      }

      if (p.burst == p.total_burst) { // Se o processo acabou o burst
#ifdef DEBUG
        printf(" (burst");
#endif
        if (p.io > 0) { // Envia o processo para a fila de I/O se ainda tiver que executar alguma I/O
#ifdef DEBUG
          printf("|io)");
#endif
          p.io--;
          p.timeout = 0;
          p.io_start_time = timer + 1;
          io.push(p);
#ifdef DEBUG
        } else {
          printf(")   ");
#endif
        }
        q0.pop();
        // Se não, o processo finalizou
      } else if (p.timeout == rr_timeout) { // Se o processo completou o tempo do Round-Robin, envia o processo para a fila q1
#ifdef DEBUG
        printf(" (rr)      ");
#endif
        p.timeout = -1;
        q1.push_back(p);
        q0.pop();
#ifdef DEBUG
      } else {
        printf("           ");
#endif
      }
    } else if (!q1.empty()) { // Se a fila q0 for vazia mas a fila q1 não for, escalona o processo dessa fila
      Process& p = q1.front();
      p.burst++;
      p.timeout = -1;

#ifdef DEBUG
      printf(" | Q1 %d", p.pid);
#endif

      // Adiciona no diagrama de Gantt
      if (gantt_pid.empty() || gantt_pid.back() != p.pid) {
        gantt_pid.push_back(p.pid);
        gantt_time.push_back(timer);
      }

      if (p.burst == p.total_burst) { // Se o processo acabou o burst
#ifdef DEBUG
        printf(" (burst");
#endif
        if (p.io > 0) { // Envia o processo para a fila de I/O se ainda tiver que executar alguma I/O
#ifdef DEBUG
          printf("|io)");
#endif
          p.io--;
          p.timeout = 0;
          p.io_start_time = timer + 1;
          io.push(p);
#ifdef DEBUG
        } else {
          printf(")   ");
#endif
        }
        q1.erase(q1.begin());
        // Se não, o processo finalizou
#ifdef DEBUG
      } else {
        printf("           ");
#endif
      }
    } else {
#ifdef DEBUG
      printf(" | (none)         ");
#endif

      // Adiciona no diagrama de Gantt
      if (gantt_pid.empty() || gantt_pid.back() != -1) {
        gantt_pid.push_back(-1);
        gantt_time.push_back(timer);
      }
    }

    // Atualiza os timeouts da fila q1
#ifdef DEBUG
    printf(" |");
#endif
    for (int i = 0; i < q1.size(); ++i) {
      q1[i].timeout++;
      if (q1[i].timeout == q1_timeout) {
#ifdef DEBUG
        printf(" %d", q1[i].pid);
#endif
        q0.push(q1[i]);
        q0.back().timeout = 0;
      }
    }
    for (int i = q1.size()-1; i >= 0; --i) {
      if (q1[i].timeout == q1_timeout)
        q1.erase(q1.begin() + i);
    }

    if (!io.empty() && io.front().io_start_time <= timer) {
      Process& p = io.front();
      p.timeout++;
#ifdef DEBUG
      printf(" | %d", p.pid);
#endif
      if (p.timeout == io_timeout) {
#ifdef DEBUG
        printf(" (timeout)");
#endif
        p.burst = 0;
        p.timeout = 0;
        q0.push(p);
        io.pop();
      }
#ifdef DEBUG
    } else {
      printf(" | ");
#endif
    }
#ifdef DEBUG
    printf("\n");
#endif

    timer++;
  }

  printf("\n\n >> Diagrama de Gantt <<\n");
  printf("Tempo\tProcesso\n");
  for (int i = 0; i < gantt_pid.size(); ++i) {
    printf("%4d\t", gantt_time[i]);
    if (gantt_pid[i] >= 0)
      printf("%2d\n", gantt_pid[i]);
    else
      printf(" (none)\n");
  }
  printf("%4d\t (finished)\n", timer);
}
