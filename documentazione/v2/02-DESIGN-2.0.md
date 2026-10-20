# Design della 2.0

Questo documento descrive il gioco desiderato, senza prescrivere come modificarne il codice. Tutte le nuove meccaniche sono in progetto.

## D01 — Obiettivi

Concordato: mondo molto più grande e ricco; azioni fluide e comprensibili; eserciti ampliati; basi fino al IV; maggiore preparazione difensiva; battaglie navali sostanziali; poteri forti con contromisure e costi. Regole economiche e limiti uguali per giocatore e IA. Le differenze estetiche non devono nascondere informazioni essenziali.

## D02 — Mappe, foreste e mare

Concordato: molte più foreste abbattibili, radure, risorse distribuite, spazio per nuove basi e numerosi teatri navali. Proposta misurabile iniziale: mappe avanzate 7680×4800, il doppio per lato dell'attuale massimo e quattro volte la superficie; valore da confermare con fluidità e tempi di attraversamento, non limite tecnico già disponibile. Prime missioni più piccole o accessibili per settori. Una mappa grande deve contenere luoghi utili, non solo distanze vuote.

Mari collegati, baie, stretti, isole, più coste edificabili, porti alternativi e risorse contendibili. Missioni di scorta, assalto ai porti, controllo di rotte e sbarchi, dopo il tutorial navale. La flotta deve incidere sull'esito e non essere un semplice requisito numerico. Garantire passaggi per navi grandi, sbarchi su terreno libero e avversari capaci di usare il mare.

Foreste irregolari con ingressi raggiungibili e abbattimento che apre percorsi. Niente rigenerazione gratuita automatica: rimane la ricerca per ripiantare. Risorse finite sufficienti a obiettivi, perdite ragionevoli e recupero economico; evitare che l'ultima caverna sia raggiungibile solo con un mezzo che richiede i suoi cristalli.

Basi iniziali curate missione per missione: spazio di espansione, uscite per le truppe, accessi a miniere e alberi, vie di difesa e piazzole portuali. Restano vincoli di terreno, costa e sovrapposizione; maggiore libertà non significa poter costruire sul mare con edifici terrestri.

## D03 — Direzione artistica e animazioni

Concordato: migliorare erba, terre, strade, acqua e transizioni; rive naturali, schiuma leggibile, onde contenute, varietà senza perdere visibilità. Sagome e colori distinguono stirpi, classi e livello degli edifici. Nuovi materiali devono accordarsi alla scala delle unità esistenti.

Azioni richieste: attesa, cammino, raccolta, trasporto, consegna, costruzione, riparazione, attacco, lancio magia, danno, morte, imbarco/sbarco e trasformazione. Movimenti con orientamenti coerenti, svolte credibili, gruppi che non si accalcano; ali e cavalcature con cicli propri. Le animazioni non devono ritardare arbitrariamente un ordine urgente.

Edifici I–IV con silhouette e dettagli progressivi, cantiere durante i lavori e completamento visibile. Ogni coppia nuova richiede scheda artistica, icona, scala, ombra, selezione e varianti di stato; non solo un'immagine illustrativa. Riferimento fantasy originale, senza recuperare asset Blizzard.

## D04 — Economia, cristalli e specialisti

Quattro risorse: oro, legno, olio, cristalli. Cristalli dell'Aurora e dell'Abisso sono varianti di fazione della stessa funzione economica. Caverne finite con scorta visibile. Alla chiusura la risorsa non è più selezionabile; l'ingresso decorativo non deve occupare terreno edificabile senza motivo. La forma precisa del residuo è da approvare artisticamente.

| Specialista | Controparte | Compito |
| --- | --- | --- |
| Minatore dell'Aurora | Scavatore dell'Abisso | Estrazione e consegna dei cristalli |

Artwork e animazioni dedicati: piccone, lanterna/carico, cammino, estrazione e consegna. Proposta: reclutamento e deposito nel Reliquiario dell'Aurora / Reliquiario dell'Abisso; costruzione del reliquiario da parte degli operai comuni. Avvio con oro e legno, nessun cristallo necessario a produrre il primo specialista. Popolazione consumata, nessuna costruzione o riparazione da parte dello specialista. Precisare in bilanciamento se la caverna è utilizzabile da entrambe le stirpi; proposta: sì, il materiale viene raffinato secondo la propria tradizione.

Impieghi: unità speciali, rune, poteri e sviluppo IV. Le unità comuni rimangono utili e non richiedono tutte la nuova risorsa. Mana e cristalli hanno funzioni diverse: costo di preparazione economico, lancio tattico normalmente legato a mana/ricarica.

## D05 — Livelli IV e supporto economico

Concordato: progressione degli edifici fino al IV, maggiori bonus e resa; Comando avanzato riduce tempi di raccolta e costruzione. Ogni tipo costruibile avrà una matrice I–IV da approvare; eccezioni per palizzate/piattaforme e forme delle torri vanno esplicitate, non dimenticate. Le risorse naturali non salgono di livello.

Proposta: livello del Comando abilita il tetto tecnologico; ogni edificio si migliora singolarmente. Bonus globali del Comando basati sul migliore completato, mai moltiplicati dal numero di comandi. Stabilire cosa resta dopo la sua distruzione: proposta, conservare le conoscenze ma perdere l'aura operativa collegata al comando. Nessuna formula definitiva qui.

Economia: distinguere quantità per carico, durata di estrazione, velocità di viaggio e tempo del cantiere. I bonus devono essere limitati e leggibili; nessun tempo nullo, risorsa infinita o raddoppio involontario. Fattorie e popolazione rimangono vincoli anche per l'IA; il nuovo tetto di popolazione è da bilanciare, non automaticamente illimitato.

Stele sacra / Stele dell'Abisso: supporto militare ed economico locale. Proposta: bonus militare alle unità nel raggio, bonus economico alle consegne nei depositi coperti. Steli sovrapposte non sommano lo stesso effetto; non creano risorse dal nulla. Raggio, costi e bonus da provare.

## D06 — Ruoli automatici e difesa

Operai comuni con ruolo lavoratore oppure riparatore. Riparatore assegnato a una zona: cerca danni, verifica risorse e raggiungibilità, ripara e torna disponibile. Un cantiere già assunto mantiene l'operaio impegnato; ordine manuale e cambio ruolo hanno priorità esplicita. Proposta: riparazioni automatiche sospese sotto soglia di risorse configurabile e niente inseguimento fuori zona.

Guerrieri: Presidia (ritorno alla posizione), Pattuglia (più punti e ripresa dopo lo scontro), Difendi il campo (riserva di zona). Torre di guardia rileva minacce e richiama soltanto difensori assegnati. Raggio d'allarme e limite d'inseguimento visibili; avamposti separati, niente esercito intero attirato da un'esca. Unità invisibili richiedono rilevamento; nessuna conoscenza della mappa regalata all'IA.

## D07 — Roster nuovo o rielaborato

Nomi e dettagli di produzione sono proposte; ruoli richiesti dall'utente fanno parte della traccia. Non fissare il conteggio finale prima di risolvere sostituzioni e doppioni.

| Umani | Ruolo | Stato |
| --- | --- | --- |
| Domatore di ipogrifi | Intercettazione aerea veloce | Nuovo |
| Cavaliere sacro | Cavalleria pesante protettiva | Proposta evoluzione del Custode esistente |
| Cavaliere dell'Aurora | Destriero spirituale, mobilità e soccorso | Nuovo; attraversamento di alleati proposto, mai muri/acqua |
| Angelo minore | Supporto aereo e cura | Nuovo; natura narrativa da definire |
| Paladino | Fanteria pesante con scudo e aura | Nuovo; distinto dal cavaliere |
| Arca dell'Aurora | Trasporto aereo fantasy | Nuovo |
| Minatore dell'Aurora | Cristalli | Nuovo |
| Spezzarune | Contrasto a barriere ed effetti runici | Nuovo |
| Vincolatore runico | Controllo e paralisi | Nuovo |

| Oscuri | Ruolo | Stato |
| --- | --- | --- |
| Drago demoniaco | Volante pesante da assedio | Nuovo |
| Predatore su bestia dell'Abisso | Cavalleria da sfondamento | Nuovo; rapporto col Cavaliere infernale da definire |
| Demone minore | Fanteria leggera aggressiva | Proposta evoluzione del Diavolo guerriero |
| Vascello dell'Abisso | Trasporto aereo fantasy | Nuovo |
| Scavatore dell'Abisso | Cristalli | Nuovo |
| Profanatore del Velo | Contrasto a barriere ed effetti runici | Nuovo |
| Carceriere dell'Abisso | Controllo e paralisi | Nuovo |

Arca e Vascello: scafi magici, vele e cristalli, niente astronavi tecnologiche. Sei posti come proposta, costo elevato, volo lento, vulnerabilità contraerea, sbarco solo su terreno libero. Sbarco protetto umano / furioso oscuro sono proposte da confrontare con rune e altari, non obblighi già bilanciati. Evitare bombardamento pesante sul medesimo trasporto. Non è necessario avere lo stesso numero di unità nuove, ma ogni vantaggio deve avere una risposta.

## D08 — Altari di barriera e contromisure

Altare della Protezione / Altare del Velo Abissale. Concordato: protezione di circa **10 secondi**, contromisure per entrambe le stirpi. Proposta: cupola locale contro danni nemici, costo in cristalli, ricarica lunga, attivazione manuale o automatica all'allarme. Definire separatamente se l'altare protegge sé stesso e se consente di sparare dall'interno; non presumerne l'equilibrio.

Protezione condivisa di zona impedisce catene infinite da altari sovrapposti. Navi protette solo se nel raggio costiero. Proposta: Spezzarune/Profanatore devono avvicinarsi e completare un rituale interrompibile; barriera spezzata termina e consuma la ricarica. Il rituale deve funzionare senza fare prima danno a un bersaglio invulnerabile. Gli specialisti possono viaggiare nei trasporti.

## D09 — Rune consumabili

Laboratorio delle Rune / Forgia dei Sigilli. Produzione con oro e cristalli, tempo e scorta limitata. Selezione squadra, scelta della runa, anteprima di costo e conferma. Rapidità, Forza, Vigore, Luce sacra umana e Furia abissale oscura sono il catalogo proposto. Definire cosa fanno i danni sacri/oscuri prima di usare bonus contro una categoria.

Effetto temporaneo distinto dalle ricerche permanenti. Una runa per unità, nessun accumulo illimitato, consumo proporzionale ai destinatari validi. Anteprima esclude bersagli non compatibili. Nessun pagamento per bersagli non raggiunti se l'azione viene rifiutata; rimborso e interruzioni da definire prima dello sviluppo. Proposta: unità mobili soltanto, non edifici. Dissoluzione da specialisti con ricarica dedicata. Regole identiche per l'IA.

## D10 — Trasfigurazione, corruzione e guarigione

Concordato: il giocatore sceglie chi potenziare; umani assumono forma oscura, oscuri forma umana. Il potere è temporaneo, rischioso e curabile anche dopo la perdita di controllo. Una lunga ricarica impedisce l'abuso. Aspetto, fedeltà e controllo sono concetti distinti: cambiare forma non trasferisce automaticamente l'unità al nemico.

Proposte di strutture: Santuario della Trasfigurazione / Altare della Trasfigurazione, entrambi con preparazione e ripristino. Non confondere questi edifici con gli altari della barriera o con i santuari esistenti.

Ciclo proposto: normale → preparato → trasformato controllabile → instabile → corrotto incontrollabile → contenuto → in trattamento → normale in recupero → nuovamente disponibile. È possibile iniziare il trattamento prima della corruzione, rinunciando al bonus. La scadenza del potere non deve essere una cura gratuita: l'instabilità residua richiede trattamento secondo regole da fissare.

Indicatore leggibile di rischio e tempo; soglia deterministica preferibile a una perdita arbitraria non annunciata. Proposta: corrotto ostile a entrambe le fazioni, con proprietario originario ricordato. Nessun normale ordine può guidarlo alla cura. Servono contenimento non letale e scorta: strumenti, distanza, resistenza e numero di scorte restano da decidere. La struttura cura anche una corruzione già completa, a costo e tempo maggiori se necessario. Non presentare come approvata la vecchia ipotesi di corruzione irreversibile: è stata superata dalla richiesta dell'utente.

Posti di trattamento limitati, nessuna azione militare durante la cura, cristalli spesi per preparazione; costo del trattamento da bilanciare. Ricarica individuale lunga parte dalla guarigione e non viene azzerata da purificazione anticipata, cambio forma, trasporto o salvataggio. Proposte numeriche: 20–30 s di potere, 15–20 s di cura, 3–5 min di recupero; nessuna è definitiva. Niente accumulo con rune di potenziamento. Primo gruppo di compatibilità: militari terrestri selezionati, non lavoratori, navi o macchine. Definire la gestione della morte, perdita della struttura e sconfitta con corrotti ancora presenti; proposta di sicurezza: garantire una struttura ricostruibile senza cristalli per non rendere il recupero economicamente impossibile.

## D11 — Paralisi e protezione

Vincolatore runico / Carceriere dell'Abisso: controllo temporaneo di un piccolo gruppo, riferimento richiesto **10 secondi**. Proposta: blocca movimento, attacco e magie; lancio annunciato e interrompibile, mana e ricarica lunghi, immunità temporanea dopo l'effetto per evitare blocco concatenato. Capi e unità potenti con resistenza/durata ridotta; mare ed edifici esclusi dal primo perimetro proposto.

Obelisco della Libertà / Pilastro della Volontà proteggono alleati nel raggio. Proposta iniziale: immunità alla paralisi nella zona; costo di costruzione e funzionamento da definire. Non introdurre una quinta risorsa chiamata energia senza progetto economico: eventuale funzionamento usa cristalli, mana o ricarica già esistenti. Sovrapposizione non aumenta il beneficio. Stabilire se entrare nella zona libera anche una paralisi già attiva: proposta sì. Distruggere protezione, aggirarla o attirare truppe fuori raggio sono contromosse leggibili.

## D12 — Priorità e chiarezza degli effetti

Proposta per discussione: contenimento/trattamento vietano altre azioni; paralisi sospende azioni ma non cura né blocca l'instabilità; barriere bloccano danni ma non automaticamente dissoluzione o trasfigurazione. Gli specialisti non purificano gratis la corruzione usando una semplice dissoluzione. Tutte queste interazioni devono avere una matrice verificata prima del combattimento combinato. Le durate continuano durante trasporto; pausa del gioco ferma la simulazione.

## D13 — Campagna e accessibilità

Nuove regole insegnate una alla volta e poi combinate. Ogni missione: obiettivo principale, sconfitta, risorse minime, prerequisiti, preparazione nemica, recupero dopo perdite e meccanica introdotta. Nessun raid devastante prima del tutorial. Proposta: percorso narrativo oscuro autonomo con obiettivi diversi; numero di nuove missioni da approvare, senza promettere altre undici mappe per parte. IT/EN per tutti i nomi, descrizioni, allarmi e tutorial; forma e simbolo accompagnano il colore.

## D14 — Criteri di riuscita

Mappe ampie percorribili a terra/mare/aria; alberi eliminati aprono spazio; nessun blocco economico sui cristalli; tutti i livelli visibili; bonus non duplicati; riparatori spendono risorse; difensori rientrano; allarme locale; navi determinanti negli scenari navali; barriere spezzabili; paralisi contrastabile; corruzione curabile prima e dopo la perdita di controllo; cooldown persistenti; IA paga le medesime azioni. Fluidità su PC di riferimento da dichiarare, senza fissare requisiti hardware prima delle misure. Ogni milestone richiede prova giocata oltre ai controlli automatici.
