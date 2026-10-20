# Third-party notices and provenance

## Modified game and engine code

This project contains modified and derived code from Warcraft-II-HD by lori2001 and NGin by Szoke Lorand.
Upstream: https://github.com/lori2001/Warcraft-II-HD (GNU GPL v3).
This distribution is a separate modified work with different gameplay, original artwork and its own title.
Keep upstream copyright and license notices when redistributing. The GPL text is in LICENSE and
progetto/licenze/GPL-3.0.txt. No Blizzard sprites, sounds, maps or fonts are included.

## Original artwork

The bitmap artwork in progetto/assets and source artwork in grafica were generated for this game
using integrated AI image generation tools. The prompts and generation notes are included in grafica.
No claim is made to exclusive copyright in purely AI-generated elements. For any project-authored
copyrightable additions, distribution follows the project’s GPL v3 terms, without extending those terms
to third-party components. This is not an official Warcraft or Blizzard product.

## SFML 2.5.1

Copyright (C) 2007-2018 Laurent Gomila. zlib/png license: progetto/licenze/SFML.txt.
Headers, import libraries and DLLs: progetto/vendor/SFML-2.5.1.
Official corresponding source archive: progetto/vendor/sorgenti/SFML-2.5.1-sources.zip.
SFML graphics uses FreeType and stb_image. This software is based in part on the work of the FreeType Team.
Notices: progetto/licenze/FreeType-FTL.txt and progetto/licenze/stb_image.txt.

## Microsoft Visual C++ runtime and Windows fonts

Redistributable runtime DLLs remain Microsoft components, not GPL code.
See progetto/licenze/Microsoft-Runtime.txt. They are installed beside the application, not into system folders.
The game loads Segoe UI or Arial from Windows. No Microsoft fonts are distributed.
Visual Studio and the Inno Setup compiler are external build tools, not included in this repository.

## Corresponding source

The release Source.zip contains the same public source files listed in SOURCE-MANIFEST.txt,
including build scripts, artwork and required dependencies. The installer and portable game also
include that source archive under sorgenti. No keys or passwords are required to extract it.
