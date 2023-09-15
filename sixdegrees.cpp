#include <iostream>
#include <stack>
#include <string>
#include <cstring>
#include <fstream>
#include <list>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <limits>
#include <algorithm>
using namespace std;

class Path {
  public:
    string prev_actor = "";
    string movie = "";
    int cost = std::numeric_limits<int>::max();
};

class Element{
  public:
    string prev_actor = "";
    string movie = "";
    int cost = 0;
};

class Pair {
  public:
    string actor = "";
    string movie = "";
};

string printPath(unordered_map<string, Path> p, string source, string target) {
  string res = "";
  vector<Pair> d;
  string movie = "";
  while (true) {
    if (target == source) {
      Pair x;
      x.actor = target;
      x.movie = movie;
      d.push_back(x);
      break;
    }
    Pair x;
    x.actor = target;
    x.movie = movie;
    d.push_back(x);
    movie = p[target].movie;
    target = p[target].prev_actor;
  }
  reverse(d.begin(), d.end());
  for (auto i: d) {
    if (i.movie == "") {
      res += i.actor;
    }
    else {
      res += i.actor + " -(" + i.movie + ")- ";
    }
  }
  return res;
}

string sixdegrees(unordered_map<string, unordered_set<string>> A, unordered_map<string, vector<vector<string>>> M, string actor1, string actor2) {
  if ((A.find(actor1) == A.end()) || (A.find(actor2) == A.end())) {
    return "Not present";
  }
  else if (actor1 == actor2) {
    return actor1;
  }
  queue<Element> q;
  unordered_map<string, Path> path;
  Element f;
  f.prev_actor = actor1;
  q.push(f);
  while (!q.empty()) {
    Element e = q.front();
    q.pop();
    //get all neighbors
    for (auto movie: A[e.prev_actor]) {
      for (auto actors: M[movie]) {
        if (find(actors.begin(), actors.end(), e.prev_actor) != actors.end()) {
          for (auto actor: actors) {
            //if visited do nothing
            if (path.find(actor) == path.end()) {
            //make new element to add to queue
            //add changes to path map
              path[actor].prev_actor = e.prev_actor;
              path[actor].movie = movie;
              path[actor].cost = e.cost + 1;
              if (actor == actor2) {
                break;
              }
              Element n;
              n.prev_actor = actor;
              n.movie = movie;
              n.cost = e.cost + 1;
              q.push(n);
            }
          }
        }
      }
    }
  }
  if (path[actor2].prev_actor == "") {
    return "Not present";
  }
  return printPath(path, actor1, actor2);
}

int main(int argc, char ** argv) {
  if (argc < 3) {
    throw std::invalid_argument("incorrect file format");
  }
  // build graph from DB
  ifstream dataBase;
  dataBase.open("cleaned_movielist.txt");
  string line;
  //  Actors<string Actor, set Movies>
  //  Movies<string Movie, set Actors>
  unordered_map<string, unordered_set<string>> Actors;
  unordered_map<string, vector<vector<string>>> Movies;
  //M["With_This_Ring"] = {{"a", "b", "c"}, "d", "e", "f"}
  while (getline(dataBase, line)) {
    if (line.length() == 0) {
      continue;
    }
    stringstream ss(line);
    string tok;
    char delimiter = ' ';
    string movie;
    getline(ss, tok, delimiter);
    movie = tok;
    vector<string> actors;
    while (getline(ss, tok, delimiter)) {
      //Insert all actors to actor list
      actors.push_back(tok);
      //Insert movie into actor's movie list
      if (Actors.find(tok) == Actors.end()) {
        //Actor not in actor map
        unordered_set<string> actor_movies;
        actor_movies.insert(movie);
        Actors[tok] = actor_movies;
      }
      else {
        //Actor in actor map
        Actors[tok].insert(movie);
      }
    }
    //Insert all actors to movie list
    Movies[movie].push_back(actors);
  }
  dataBase.close();
  //Perform Sixdegrees Shortest Path Algorithm from Input
  ifstream input;
  ofstream output;
  
  input.open(argv[1]);
  output.open(argv[2]);
  string word;
  char *whole, *first, *second;
  while (getline(input, word)) {
    if (word.length() == 0) continue;
    whole = strdup(word.c_str());
    first = strtok(whole, " \t");
    second = strtok(NULL, " \t");
    string res = sixdegrees(Actors, Movies, first, second);
    output << res << endl;
  }
  input.close();
  output.close();
  //string res = sixdegrees(Actors, Movies, "Peter_Greene", "Alice_Ensor");
  //cout << res << endl;
  return 0;
}
