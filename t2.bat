@echo off
powershell -NoProfile -Command "foreach ($root in 'src', 'include') { if (Test-Path $root) { Get-ChildItem -Path $root -Recurse -File -ErrorAction SilentlyContinue | ForEach-Object { $relPath = $_.FullName.Replace((Get-Location).Path + '\', '').Replace('\', '/'); [Console]::WriteLine('{0} -- {1} B', $relPath, $_.Length) } } }"
