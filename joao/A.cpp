#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/*
 * Enunciado: 
 * M turmas e N tipos de frutas diferentes
 *
 * m: Turma -> para cada fruta x
 *                        ^-- alunos que gostam.
 *                        e.g. Fruta {liked_by: ['aluno1', 'aluno2', 'aluno3']}
 *
 * Porem nao temos dados individuais de alunos.
 *    e.g. Aluno { likes: ['fruta1', 'fruta2', 'fruta3'] }
 *
 * E tambem nao sabemos quantos alunos ha em cada turma.
 * 
 * Problema:
 * Com base nos dados das frutas(quantos alunos gostam de cada fruta EM CADA TURMA)
 * Determinar o menor numero POSSIVEL de alunos que a escola pode ter.
 * Sabendo que:
 *  - cada turma tem pelo menos 1 aluno;
 *  - cada aluno pertence a unica turma;
 *  - cada aluno gosta de PELO MENOS 1 fruta;
 *  - um mesmo aluno pode gostar de varias frutas;
 *
 * Escola{turmas: Turma[]}
 * Turma{alunos: Aluno[min:1]}
 * Aluno{gosta: Fruta[min:1]}
 * Fruta{gostado_por: Aluno[]}
 *
 * Input:
 * Primeira linha:
 * <integer> <integer> => N e M (N >= 1, M <= 1000) => numero de frutas(N) numero de turmas(M)
 * Cada uma das {N} linhas seguintes(frutas) contem {M} (numero de alunos) que gostam naquela turma.
 * formando uma tabela G[i][j]
 *                               Turma 1   Turma 2   Turma 3
 *                       Fruta 1      20        15        14
 *                       Fruta 2      12        20        12
 *                       Fruta 3      18         5        10
 * Estrategia:
 * Encontrar o maior numero em cada coluna(turma);
 * Este numero sera o numero minimo de alunos nesta turma.
 * Somar cada um desses numeros minimos de cada turma 
 * e entao encontramos o numero minimo de alunos na escola.
 *
 */

int sum(vector<int>& vals) {
    return accumulate(vals.begin(), vals.end(), 0);
}

int main(int argc, char** argv) {
    int N, M;
    cin >> N >> M; cin.ignore();

    vector<int> max_by_turma(M, 0);

    string line;
    while(getline(cin, line)) {
        istringstream iss(line);
        for (int i = 0; i < max_by_turma.size(); ++i) {
            int value; iss >> value;
            if (value > max_by_turma[i]) max_by_turma[i] = value;
        }
    }

    int total = sum(max_by_turma);
    printf("%d\n", total);

    return 0;
}
