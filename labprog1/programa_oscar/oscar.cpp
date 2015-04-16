#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

enum Genre
{
  GENRE_ACAO,
  GENRE_AVENTURA,
  GENRE_COMEDIA,
  GENRE_HORROR,
  GENRE_ROMANCE,
  GENRE_DRAMA,
  GENRE_SCIFI,

  GENRE_NUM // Internal usage
};

std::string genre_names[GENRE_NUM] = {
  "Acao",
  "Aventura",
  "Comedia",
  "Horror",
  "Romance",
  "Drama",
  "Sci-fi"
};

void listGenres()
{
  std::cout << "Generos:\n";
  for (int i = 0; i < GENRE_NUM; ++i)
    std::cout << i + 1 << ": " << genre_names[i] << "\n";
  std::cout << std::endl;
}

enum Indication
{
  INDICATION_FILME,
  INDICATION_ATOR,
  INDICATION_ATRIZ,
  INDICATION_DIRETOR,
  INDICATION_ATOR_COADJ,
  INDICATION_ROTEIRO,
  INDICATION_FOTOGRAFIA,
  INDICATION_MIXAGEM_SOM,
  INDICATION_EDICAO_SOM,
  INDICATION_TRILHA_SONORA,
  INDICATION_FIGURINO,
  INDICATION_MAQUIAGEM,
  INDICATION_EDICAO,

  INDICATION_NUM
};

std::string indications_names[INDICATION_NUM] =
{
  "Filme",
  "Ator",
  "Atriz",
  "Diretor",
  "Ator Coadjuvante",
  "Roteiro",
  "Fotografia",
  "Mixagem de Som",
  "Edicao de Som",
  "Trilha Sonora",
  "Figurino",
  "Maquiagem",
  "Edicao"
};

void listIndications()
{
  std::cout << "Indicacoes: \n";
  for (int i = 0; i < INDICATION_NUM; ++i)
    std::cout << i + 1 << ": " << indications_names[i] << "\n";
  std::cout << std::endl;
}

struct  Movie
{
  std::string name, name_pt;
  int qntIndications;
  std::vector<Indication> indications;
  Genre genre;
  int evaluation;
};

struct MovieCatalog
{
  std::vector<Movie> movies;

  void insertMovie(Movie movie)
  {
    movies.push_back(movie);
  }

  void removeMovieByName(const char* name)
  {
    for (unsigned int i = 0; i < movies.size(); ++i)
    {
      if (movies[i].name.compare(name) == 0)
      {
        movies.erase(movies.begin() + i);
        return;
      }
    }
  }

  void removeMovieByIndex(int index)
  {
    movies.erase(movies.begin() + index);
  }

  bool hasMovies() const
  {
    if (movies.size() == 0)
    {
      std::cout << "Nenhum filme!\n" << std::endl;
      return false;
    }

    return true;
  }

  bool listMoviesNames()
  {
    if (!hasMovies())
      return false;

    for (unsigned i = 0; i < movies.size(); ++i)
      std::cout << i + 1 << ": " << movies[i].name << "\n";
    std::cout << std::endl;

    return true;
  }

  void listMovie(int i)
  {
    std::cout << "Filme " << i + 1 << ":" << "\n"
              << "Nome: " << movies[i].name << "\n"
              << "Nome em portugues: " << movies[i].name_pt << "\n"
              << "Indicacoes: " << movies[i].qntIndications << "\n";

    for (int j = 0; j < movies[i].qntIndications; ++j)
      std::cout << "> " << indications_names[movies[i].indications[j]] << "\n";

    std::cout << "Genero: " << genre_names[movies[i].genre] << "\n"
              << "Avaliacao pessoal: " << movies[i].evaluation << "\n"
              << std::endl;
  }

  void listMovies()
  {
    if (!hasMovies())
      return;

    for (unsigned int i = 0; i < movies.size(); ++i)
      listMovie(i);
  }

  void listMoviesByGenre(Genre genre)
  {
    if (!hasMovies())
      return;

    int cnt = 0;
    for (unsigned int i = 0; i < movies.size(); ++i)
    {
      if (movies[i].genre == genre)
      {
        listMovie(i);
        cnt++;
      }
    }

    if (cnt == 0)
      std::cout << "Nenhum filme do genero " << genre_names[genre]
                << "!\n" << std::endl;
  }

  void listMoviesByEvaluation(int evalution, bool is_greater)
  {
    if (!hasMovies())
      return;

    int cnt = 0;
    for (unsigned int i = 0; i < movies.size(); ++i)
    {
      if (( is_greater && movies[i].evaluation >= evalution) ||
          (!is_greater && movies[i].evaluation <= evalution))
      {
        listMovie(i);
        cnt++;
      }
    }

    if (cnt == 0)
      std::cout << "Nenhum filme com avaliacao "
                << (is_greater ? "maior" : "menor")
                << " ou igual a " << evalution << ".\n" << std::endl;
  }

  bool saveToFile()
  {
    //std::ofstream file {"catalog.m"}; // C++11 only...
    std::ofstream file ("catalog.m");

    if (!file.good())
    {
      std::cout << "Error ao abrir arquivo catalog.m\n" << std::endl;
      return false;
    }

    file << movies.size() << "\n";

    int ms = movies.size();
    for (int i = 0; i < ms; ++i)
    {
      file << movies[i].name.length() << "\n";
      file << movies[i].name << "\n";

      file << movies[i].name_pt.length() << "\n";
      file << movies[i].name_pt << "\n";

      file << movies[i].qntIndications << "\n";
      int is = movies[i].qntIndications;
      for  (int j = 0; j < is; ++j)
        file << static_cast<int>(movies[i].indications[j]) << "\n";
      file << static_cast<int>(movies[i].genre) << "\n";
      file << movies[i].evaluation << "\n";
    }

    file.close();

    return true;
  }

  bool loadFromFile()
  {
    //std::ifstream file {"catalog.m"}; // C++11 only...
    std::ifstream file ("catalog.m");

    if (!file.good())
    {
      std::cout << "Error ao abrir arquivo catalog.m\n" << std::endl;
      return false;
    }

    int ms;
    file >> ms;

    movies.resize(ms);
    for (int i = 0; i < ms; ++i)
    {
      int len;
      file >> len;
      file.ignore(8, '\n');
      std::getline(file, movies[i].name);

      file >> len;
      file.ignore(8, '\n');
      std::getline(file, movies[i].name_pt);

      file >> movies[i].qntIndications;

      int is = movies[i].qntIndications;
      movies[i].indications.reserve(is);

      int ind;
      for (int j = 0; j < is; ++j)
      {
        file >> ind;
        movies[i].indications.push_back(static_cast<Indication>(ind));
      }

      int genre;
      file >> genre;
      movies[i].genre = static_cast<Genre>(genre);
      file >> movies[i].evaluation;
    }

    file.close();

    return true;
  }

  void showMenu()
  {
    std::cout << "### OSCAR Menu ###" << "\n"
              << "1: Inserir filme" << "\n"
              << "2: Remover filme" << "\n"
              << "3: Listar filmes" << "\n"
              << "4: Gravar filmes" << "\n"
              << "5: Recuperar filmes" << "\n"
              << "6: Listar filmes por gênero" << "\n"
              << "7: Listar filmes com avaliacao menor" << "\n"
              << "8: Listar filmes com avaliacao maior" << "\n"
              << "9: Fechar programa" << "\n"
              << std::endl;
  }

  bool menu()
  {
    showMenu();

    int opt;
    std::cout << "Opcao: "; std::cin >> opt;
    std::cin.ignore(8, '\n');
    std::cout << std::endl;

    int temp;
    if (opt == 1)
    {
      std::cout << "-- Inserir filme --\n\n";

      Movie m;
      std::cout << "Nome do filme original: "; std::getline(std::cin, m.name);
      std::cout << "Nome do filme em portugues: "; std::getline(std::cin, m.name_pt);

      std::cout << "Quantidade de indicacoes: "; std::cin >> m.qntIndications;
      if (m.qntIndications > 0)
      {
        std::cout << "\n";
        listIndications();

        std::cout << "Indicacoes: ";
        for (int i = 0; i < m.qntIndications; ++i)
        {
          std::cin >> temp;
          m.indications.push_back(static_cast<Indication>(temp - 1));
        }
      }
      std::cout << "\n";

      listGenres();
      std::cout << "Genero: "; std::cin >> temp;
      m.genre = static_cast<Genre>(temp);

      std::cout << "Avaliacao pessoal (1~5): "; std::cin >> m.evaluation;
      std::cout << std::endl;

      insertMovie(m);
    }
    else if (opt == 2)
    {
      std::cout << "-- Remover filme --\n\n";
      if (listMoviesNames())
      {
        std::cout << "Remover filme: "; std::cin >> temp;
        std::cout << std::endl;

        if (temp <= 0 || temp > (int)movies.size())
          std::cout << "Filme invalido!\n" << std::endl;
        else
        {
          removeMovieByIndex(temp - 1);
          std::cout << "Filme removido com sucesso!\n" << std::endl;
        }
      }
    }
    else if (opt == 3)
    {
      std::cout << "-- Lista filmes --\n\n";
      listMovies();
    }
    else if (opt == 4)
    {
      std::cout << "-- Gravar filmes --\n";
      if (saveToFile())
        std::cout << "Gravacao bem sucedida!\n" << std::endl;
    }
    else if (opt == 5)
    {
      std::cout << "-- Recuperar filmes --\n";
      if (loadFromFile())
        std::cout << "Recuperacao bem sucedida!\n" << std::endl;
    }
    else if (opt == 6)
    {
      std::cout << "-- Listar filmes por gênero --\n\n";
      if (hasMovies())
      {
        listGenres();
        std::cout << "Escolha o genero: "; std::cin >> temp;
        std::cout << std::endl;
        listMoviesByGenre(static_cast<Genre>(temp));
      }
    }
    else if (opt == 7)
    {
      std::cout << "-- Listar filmes com avaliacao menor --\n\n";
      std::cout << "Avaliacao maxima: "; std::cin >> temp;
      std::cout << std::endl;
      listMoviesByEvaluation(temp, false);
    }
    else if (opt == 8)
    {
      std::cout << "-- Listar filmes com avaliacao maior --\n\n";
      std::cout << "Avaliacao minima: "; std::cin >> temp;
      std::cout << std::endl;
      listMoviesByEvaluation(temp, true);
    }
    else if (opt == 9)
    {
      std::cout << "-- Fim de execucao --" << std::endl;
      return false;
    }

    return true;
  }
};

int main()
{
  MovieCatalog catalog;
  while (catalog.menu())
    ;
  return 0;
}
