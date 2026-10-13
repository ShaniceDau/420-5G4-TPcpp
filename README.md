# TP C++

**Nom** : _Shanice Daudier_

## Construire le projet
Vous pouvez utiliser un dev container de base C++ de VScode.
Le projet utilise cmake, pensez à l'inclure dans votre dev container.

Voici les lignes de commandes pour compiler le projet:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ cd ..
$ ./build/bibliotheque -d data
```

## Répertoire data

Il contient 2 fichiers `books.txt`et `users.txt` que vous pouvez utilisez pour tester votre code.
Pour ca il suffit de donner chemin vers le repertoire data avec l'application `bibliotheque -d <chemin verrs data>`

## Fonctionnalités implémentées

### Interface et expérience utilisateur : Afficher le nom d’utilisateur plutôt que l’id dans l’affichage des livres.

Quand un livre est emprunté, l'affichage montre maintenant le nom de la personne (exemple « emprunté par Shanice Daudier ») au lieu de son ID. Le fichier `books.txt` garde quand même l'ID, parce qu'il est unique alors que deux personnes peuvent avoir le même nom. Le nom est retrouvé seulement au moment de l'affichage, avec la méthode `getUserName()` que j'ai ajoutée dans `Library`. Si l'ID ne correspond à aucun usager, le programme affiche « Utilisateur invalide » au lieu de planter.

### Gestion des données : Tri des résultats par titre, auteur pour l’affichage 

Quand on affiche tous les livres (option 5), le programme demande maintenant si on veut les trier par titre, par auteur ou pas du tout. Les résultats de recherche (options 3 et 4) sont aussi triés. Si deux livres ont le même auteur, ils sont classés par titre.


### Journal d'activités

Les actions importantes (ajout ou suppression d'un livre, ajout d'un usager, emprunt et retour) sont notées dans le fichier `journal.log`, avec la date et l'heure. Le fichier est créé s'il n'existe pas, et les nouvelles lignes s'ajoutent à la fin, donc on garde l'historique d'une session à l'autre.

### Correction du bug de sauvegarde

Si on lançait le programme sans `--data-dir`, il essayait de sauvegarder dans un fichier sans nom et affichait une erreur peu claire. Maintenant, il vérifie d'abord qu'un dossier a été donné. Sinon, il ne sauvegarde pas et affiche un message qui explique pourquoi.

J'ai aussi corrigé un autre problème dans les constructeurs de `Book` : un livre qu'on venait d'ajouter pouvait être considéré comme déjà emprunté, et on ne pouvait pas le prêter.

## Question 1 : C++ 

- Expliquez en détails une fonctionnalité / notion dans le code que ne nous avons pas ou peu vu en cours. 

On a vu qu'en C++ il faut gérer la mémoire soi-même : chaque `new` doit avoir son `delete`, sinon on a une fuite de mémoire. Les smart pointers ont été mentionnés comme solution à ce problème, mais sans être expliqués. Le projet les utilise pourtant dans la classe `Library`.

Un `unique_ptr` est un pointeur qui libère la mémoire tout seul quand on n'en a plus besoin. Voici la différence avec un exemple simple :

```cpp
// Avec new et delete
Book* livre = new Book("Candide", "Voltaire", "123");
// ...
delete livre;   // il ne faut pas l'oublier, sinon fuite de mémoire

// Avec unique_ptr
unique_ptr<Book> livre = make_unique<Book>("Candide", "Voltaire", "123");
// ...
// pas de delete : la mémoire est libérée automatiquement
```

`make_unique` fait le même travail que `new` : il crée le livre en mémoire. La différence, c'est que le `unique_ptr` s'occupe lui-même du `delete` à la fin. On ne peut donc plus l'oublier.

Dans le projet, c'est comme ça que la bibliothèque garde ses livres :

```cpp
// library.h
vector<unique_ptr<Book>> books;

// library.cpp
void Library::addBook(const Book& book) {
    books.push_back(make_unique<Book>(book));
}
```

Chaque livre ajouté est placé dans un `unique_ptr`. Quand on supprime un livre avec `removeBook()`, ou quand le programme se termine, la mémoire est libérée automatiquement. C'est pour ça que la classe `Library` n'a pas besoin d'un destructeur avec des `delete`, contrairement au `GestionnaireFormes` de l'exercice du cours. Son destructeur est juste `~Library() = default;`.

Le mot « unique » veut dire qu'un seul pointeur est responsable de l'objet. Quand le code veut seulement lire un livre, il utilise `.get()`, qui donne un pointeur normal (`Book*`). C'est ce que fait `getAllBooks()`, et c'est cette liste que mon tri utilise. Il ne faut pas faire de `delete` sur ces pointeurs, puisque c'est le `unique_ptr` qui s'en charge.

## Question 2 : Options de développement possible

- Proposez une solution plus adaptée pour la gestion de bibliothèque et faisant appel éventuellement à une technologie autre que le C++, et expliquez comment vous interfaceriez ça avec le C++. Quelle solution technologique utiliseriez vous? Pensez au futur de cette bibliothèque à Alexandrie qui pourrait éventuellement contenir des millions de livres.

Je proposerais d'utiliser une base de données comme **PostgreSQL**. Au lieu d'un fichier `books.txt`, on aurait une table :

```sql
CREATE TABLE books (
    isbn         VARCHAR(13) PRIMARY KEY,
    title        TEXT,
    author       TEXT,
    is_available BOOLEAN,
    borrower_id  VARCHAR(10)
);
```

L'ISBN est la clé primaire, donc la base peut retrouver un livre parmi des millions presque instantanément, sans tout charger en mémoire. Plusieurs personnes peuvent aussi l'utiliser en même temps sans écraser le travail des autres. On aurait aussi des tables pour les usagers et pour le journal d'activités.

Le reste du projet n'aurait presque pas besoin de changer : il suffirait de remplacer la classe `FileManager` par une classe qui lit et écrit dans la base de données au lieu des fichiers texte. Plus tard, on pourrait aussi faire une application web, pour que les employés de toutes les succursales puissent consulter le catalogue depuis un navigateur.
