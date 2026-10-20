# Texture originali Frontiere, versione 2

Generazione con lo strumento integrato image_gen, senza API o servizi aggiuntivi configurati. Asset finale: `progetto/assets/mappe-v2/terreni-v2.png`. L'atlante viene letto in quattro quadranti direttamente dal motore; l'originale generato non è stato ritoccato.

## Prompt finale

Use case: stylized-concept. Asset type: original repeatable terrain texture atlas for Frontiere medieval fantasy RTS, painterly realistic pre-rendered 2D terrain seen straight overhead, matching muted olive grass and detailed stone buildings. One square image divided precisely into FOUR equal square quadrants 2x2, touching with NO gutters NO text NO borders. Top left: seamless warm compacted ochre brown dirt road texture, subtle wheel tracks tiny embedded pebbles uniformly spread, no road outline no surrounding grass. Top right: seamless dark teal turquoise coastal water viewed vertically overhead, small natural ripples and dappled highlights, low contrast, no shore no objects no white breaking surf. Bottom left: seamless irregular weathered light warm grey medieval stone paving, small closely fitted stones, earthy seams, no perspective. Bottom right: seamless deeper blue green ocean water, subtle flowing wavelets and caustics, no shore no boats. Each quadrant internally tileable, consistent diffuse daylight, game production texture atlas, no large objects, no labels, no strong isolated features, no characters. Rich material detail but quiet enough to place units above.

## Uso nel gioco

Quadrante superiore sinistro: terra battuta; superiore destro: acqua costiera; inferiore sinistro: selciato; inferiore destro: mare profondo. Il renderer ripete le texture, sovrappone due correnti in movimento e disegna spiaggia e schiuma lungo la costa. Le animazioni di lavoro e combattimento sono effetti del motore sopra gli sprite esistenti.
