# README

TP Noté de Reina Al Masri - LDD2 IM | Groupe 2

## **Contenu du TP**
Le TP comprend 4 fichiers :
1. `serveur_tcp.c`
2. `client_tcp.c` 
3. `serveur_udp.c`
4. `client_udp.c`


*Les fichiers de même nom avec 'v2' à la fin sont les mêmes fichiers sans les commentaires. 
Toutes les commandes on été faites dans plusieurs terminal de ma machine personnelle (sous macOS, avec le compilateur clang).*

---

## **Commandes de compilation**

- **Fichier : serveur_tcp** :
  ```bash
  clang -o serveur_tcp serveur_tcp.c
  ```
- **Fichier : client_tcp** :
  ```bash
  clang -o client_tcp client_tcp.c
  ```
- **Fichier : serveur_udp** :
  ```bash
  clang -o serveur_udp serveur_udp.c
  ```
- **Fichier : client_udp** :
  ```bash
  clang -o client_udp client_udp.c
  ```

---

## **Commandes d'exécution**

### **TCP**

**Pour lancer le serveur TCP :**
```bash
./serveur_tcp
```

**Exemple de sortie :**
```
Bienvenue sur le serveur TCP ! :D
Serveur établi avec succès. En attente de messages.
```

**Pour lancer le client TCP :**
Indiquer l'adresse IP du serveur : (127.0.0.1 pour localhost)
```bash
./client_tcp 127.0.0.1
```

**Exemple de sortie :**
```
Connexion établie avec le serveur sur le port 9600.
Tapez un message à envoyer au serveur (ou 'quit' pour fermer la connexion) : Bonjour
Message envoyé au serveur avec succès !
```

**Serveur après réception du message :**
```
Nouvelle connexion établie avec le client 127.0.0.1:XXXXX.
Message reçu : Bonjour
```

**Si le client décide de fermer la connexion :**
```
Le client a fermé la connexion.
```

---

### **UDP**

**Pour lancer le serveur UDP :**
```bash
./serveur_udp
```

**Exemple de sortie :**
```
Bienvenue sur le serveur UDP ! :D
Serveur établi avec succès. En attente de messages.
```

**Pour lancer le client UDP :**
Indiquer l'adresse IP du serveur : (127.0.0.1 pour localhost)
```bash
./client_udp 127.0.0.1
```

**Exemple de sortie :**
```
Tapez un message à envoyer au serveur : Bonjour
Vous avez tapé : Bonjour
Message envoyé au serveur avec succès !
```

**Serveur après réception du message :**
```
Message #1 reçu de 127.0.0.1:XXXXX : Bonjour
```

---

## **Fonctionnement**

### **TCP**
1. **Serveur :** attend les connexions d'un client.
2. **Client :** se connecte au serveur, envoie des messages, et peut fermer la connexion en tapant `quit`.

### **UDP**
1. **serveur :** attend les messages des clients sans connexion persistante.
2. **client :** envoie un message au serveur sans établir une connexion.

---

## **Exemples Complets**

### **Serveur TCP (Terminal 1)** :
```
Bienvenue sur le serveur TCP ! :D
Serveur établi avec succès. En attente de messages.
Nouvelle connexion établie avec le client 127.0.0.1:12345.
Message reçu : Bonjour
Le client a fermé la connexion.
```

### **Client TCP (Terminal 2)** :
```
Connexion établie avec le serveur sur le port 9600.
Tapez un message à envoyer au serveur : Bonjour
Message envoyé au serveur avec succès !
Tapez un message à envoyer au serveur (ou 'quit' pour fermer la connexion) : quit
Fermeture de la connexion.
```

### **Serveur UDP (Terminal 3)** :
```
Bienvenue sur le serveur UDP ! :D
Serveur établi avec succès. En attente de messages.
Message #1 reçu de 127.0.0.1:54321 : Bonjour
```

### **Client UDP (Terminal 4)** :
```
Tapez un message à envoyer au serveur : Bonjour
Vous avez tapé : Bonjour
Message envoyé au serveur avec succès !
```
---

### **Notes**
1. Démarrer toujours le serveur avant le client.
2.`CTRL+C` pour arrêter un serveur.

---


