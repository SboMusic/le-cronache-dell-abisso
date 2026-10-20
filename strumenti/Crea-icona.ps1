param([string]$ProjectPath=(Join-Path $PSScriptRoot '../progetto'))
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$bitmap=[System.Drawing.Bitmap]::new(64,64)
$g=[System.Drawing.Graphics]::FromImage($bitmap)
$g.Clear([System.Drawing.Color]::FromArgb(24,48,49))
$brush=[System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(222,192,125))
$g.FillRectangle($brush,12,24,40,30)
$g.FillRectangle($brush,12,12,10,18)
$g.FillRectangle($brush,27,12,10,18)
$g.FillRectangle($brush,42,12,10,18)
$dark=[System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(24,48,49))
$g.FillRectangle($dark,27,38,10,18)
$bitmap.Save((Join-Path $ProjectPath 'assets/frontiere-icon.png'),[System.Drawing.Imaging.ImageFormat]::Png)
$icon=[System.Drawing.Icon]::FromHandle($bitmap.GetHicon())
$stream=[IO.File]::Create((Join-Path $ProjectPath 'frontiere.ico'))
$icon.Save($stream);$stream.Dispose();$icon.Dispose();$g.Dispose();$brush.Dispose();$dark.Dispose();$bitmap.Dispose()

