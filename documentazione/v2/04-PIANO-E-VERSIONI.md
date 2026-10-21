# Piano, isolamento e pubblicazione

## Riferimenti preservati

- `v1.0.0`: tag pubblico immutato sul commit di release 49779c1; installer e archivi già pubblicati non vengono sostituiti.
- `release/1.x`: ramo di conservazione basato su b68c51d, include la pulizia dei nomi successiva alla release.
- `main`: resta alla base b68c51d durante questa preproduzione.
- `develop/2.0`: ramo della progettazione, in una cartella di lavoro distinta `le-cronache-dell-abisso-v2`, adiacente a `rts-frontiere`.
- `backup/prima-della-2.0.bundle` nella cartella 1.x: copia locale verificata della storia Git e degli oggetti raggiungibili prima della progettazione. Non include file ignorati, installazioni o salvataggi personali; i pacchetti 1.0 e i backup esistenti rimangono al loro posto.

La cartella 2.0 è un worktree Git: i file di lavoro sono separati, ma il database Git è condiviso. Il bundle fornisce una copia indipendente per il recupero della storia. Non spostare o cancellare manualmente `.git`; per un trasferimento usare un clone o un archivio verificato. Non rinominare la cartella 1.x per motivi estetici durante l'isolamento.

## Avvio e dati

Nessun eseguibile viene copiato o presentato come 2.0 giocabile in questo step. Il nuovo avvio di anteprima indirizza i dati a `%LOCALAPPDATA%/ChroniclesOfTheAbyss-2-preview`; non legge i profili 1.0 per impostazione predefinita. Anche l'avvio ufficiale nella sola copia 2.0 rinvia a questo avvio protetto. Dal 21 ottobre 2026 il primo incremento modifica World e introduce il formato 4, conservando la lettura dei formati 1–3.

Anche avviato direttamente, l'eseguibile di sviluppo usa ora il percorso di anteprima; FRONTIERE_DATA_DIR continua a consentire directory esplicite per i test. Gli script di test già impostano directory dedicate. Nessun accesso ai salvataggi personali è necessario alla progettazione.

La creazione di pacchetti della copia 2.0 è bloccata finché versione, metadati e isolamento dei dati non vengono aggiornati insieme. Il gioco di sviluppo dichiara 2.0.0-dev.1; lo stato del ramo è sviluppo iniziale e non deve produrre un falso installer 2.0 né sovrascrivere pacchetti 1.0.

## Milestone e condizioni di uscita

| Tappa | Risultato | Dipendenze e accettazione |
| --- | --- | --- |
| M0 — Progettazione | Inventario, design, audit, lore, registro e isolamento | Nessuna nuova meccanica; sorgenti invariati; baseline verificata |
| M1 — Fondamenta | Mappa dinamica, identità/relazioni e schema salvataggi, misure | Confini, import su copia, benchmark e test legacy |
| M2 — Territorio | Mappe ampie, foreste dense, percorsi e basi riprogettate | Navigazione terra/mare, uscite libere, tempi di attraversamento accettabili |
| M3 — Economia | Cristalli, due specialisti, depositi, livelli I–IV e steli | Nessun blocco iniziale, bonus limitati, parità economica IA |
| M4 — Automazione | Riparatori, presidio, pattuglia e allarmi locali | Priorità ordini, ritorno al posto, costi e limiti d'inseguimento |
| M5 — Arte e animazioni | Prima famiglia completa di terreno/edifici/unità, poi estensione | Confronto giocato, leggibilità, clip e transizioni; direzione artistica già avviata come studio prima di M5 |
| M6 — Eserciti e mare | Nuove unità, trasporti aerei, vere battaglie navali | Ruoli distinti, contraerea, sbarchi, porti e IA navale |
| M7 — Magia tattica | Barriere, dissoluzione, rune, paralisi e protezioni | Matrice interazioni, niente catene infinite o potenziamenti gratuiti |
| M8 — Trasfigurazione | Trasformazione, perdita controllo, recupero e cura | Recuperabile anche da ostile; cooldown persistenti; nessuna cura gratuita |
| M9 — Campagne | Progressione tutorial e arco narrativo aggiornato | Obiettivi accessibili, risorse sufficienti, primo percorso completo poi variante autonoma da dimensionare |
| M10 — Release | Bilanciamento, IT/EN, regressioni, installer e sorgenti | Test completi, prova installazione, hash, licenze, backup e decisione di pubblicazione |

Non implementare tutto insieme. Ogni tappa può avere più commit; una sola feature completa e verificabile per incremento. Numero finale di missioni, quantità di unità sostituite e valori economici vengono fissati prima delle rispettive tappe. Nessuna data di uscita inventata.

## Regole GitHub

Pubblicare il ramo develop/2.0 e i documenti, senza promuoverli a release stabile o spostare tag. Aggiornamenti 1.x solo sul suo ramo; integrazioni esplicite, niente reset distruttivi. Prima di ogni milestone: stato pulito, commit identificabile e registro aggiornato. Al primo eseguibile testabile usare un numero prerelease, per esempio 2.0.0-alpha.1, con dati separati, e marcare la release GitHub come prerelease. Il tag 2.0.0 verrà creato soltanto al completamento della versione.

Ripristino storia da bundle: clonare `prima-della-2.0.bundle` in una nuova directory e verificare i riferimenti; oppure clonare il repository GitHub e scegliere v1.0.0/release/1.x. Non effettuare un reset della cartella con lavoro non salvato. Per giocare la 1.0 usare l'installer pubblico o la cartella 1.x, non il ramo di sviluppo.

## Avanzamento al 21 ottobre 2026

M0 completata. M1 avviata, non completata: griglie dinamiche e salvataggi per mappe grandi disponibili; identità/controllo delle unità, misure di scala e schema futuro degli effetti ancora da sviluppare. Nessuna missione è stata ingrandita artificialmente. Prima prova tecnica 192×120 celle (7680×4800); limite di sicurezza attuale 256×256, non promessa di fluidità su tale dimensione. Il limite di 4096 entità e 4 MiB dei file campagna resta da riesaminare con foreste e percorsi reali.
