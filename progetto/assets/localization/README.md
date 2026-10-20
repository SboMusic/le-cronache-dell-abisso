# Localizzazione
Italiano e lingua sorgente; en.tsv contiene le traduzioni inglesi del testo visualizzato.
Formato UTF-8: originale TAB traduzione. Le sequenze letterali \n indicano ritorni a capo.
Il renderer traduce il testo completo, poi i frammenti dinamici con corrispondenza piu lunga
senza tradurre nuovamente l output. Cache limitata a 4096 stringhe per evitare lavoro ripetuto.
Il modello e i salvataggi mantengono identificatori e testo sorgente per la compatibilita.
Per aggiungere una lingua, aggiungere un catalogo e il selettore; non duplicare regole di gioco.
Test: PRESENTATION per titoli/briefing e selettore, CHEAT per messaggi variabili e console.
