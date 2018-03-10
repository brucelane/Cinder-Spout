Cinder-Spout
============

Cinder version 0.9.1 block for Spout ( http://spout.zeal.co/ )

Setup Spout 2.006 or later

Copy lib/SpoutPanel.exe in your Debug/Release compiled executable folder

For 64, modify project with:
- Cinder-Spout\lib\Spout64.lib
- post build: copy /y "..\..\..\Cinder\blocks\Cinder-Spout\lib\Spout64.dll" "$(OutDir)"Spout.dll