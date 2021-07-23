if (-not $env:qmodName) 
{
    $env:qmodName = "GOOMPS"
}  
if (-not $env:module_id)
{
    $env:module_id = "goomps"
}

$zip = $env:qmodName + ".zip"
$qmod = $env:qmodName + ".qmod"

$mod = "./mod.json"
$modJson = Get-Content $mod -Raw | ConvertFrom-Json

$allLibs = Get-ChildItem ./libs/arm64-v8a/*.so | Select -Expand Name
$libs = @()

$cover = "./" + $modJson.coverImage

$fileList = @($cover, $mod)

$bannedLibList = @("modloader", "codegen", "gorilla-utils", "monkecomputer")

$modlib = @()

foreach ($lib in $allLibs) 
{
    # ignore modloader
    $contains = 0
    foreach ($ban in $bannedLibList)
    {
        if ($lib.Contains($ban))
        {
            $contains = 1
            break
        }
    }

    if ($contains)
    {
        continue
    }
    # if its the actual mod lib
    else
    {
        if ($lib.Contains($env:module_id))
        {
            $path = "./libs/arm64-v8a/" + $lib;
            $fileList += ,$path;
            $modlib += ,$lib;
            continue
        }
    }
    $path = "./libs/arm64-v8a/" + $lib
    $fileList += ,$path
    $libs += ,$lib
}

$extraFiles = @()

if (Test-Path "./ExtraFiles")
{
    $extraEntries = Get-ChildItem ./ExtraFiles/* -Recurse

    foreach ($entry in $extraEntries)
    {
        $mode = $entry | Select -Expand Mode
        if ($mode.Contains("d"))
        {
            continue
        }

        # if not a dir
        if (-not $entry.Directory.Name.Contains("ExtraFiles"))
        {
            $dir = $entry.Directory
            $folderPath = $dir.Name + "/" + $entry.Name
            while (($dir.Directory) -and (-not $dir.Directory.Name.Contains("ExtraFiles")))
            {
                $folderPath = $dir.Directory.Name + "/" + $folderPath
            }
            $extraFiles += ,$folderPath
        }
        else
        {
            $extraFiles += ,$entry.Name
        }
    }

    foreach ($file in $extraFiles)
    {
        $path = "./ExtraFiles/" + $file
        $fileList += ,$path
    } 
}
else
{
    echo "No ExtraFiles Directory Found"
}
# update version from qpm json
$qpm = "./qpm.json"
$qpmJson = Get-Content $qpm | ConvertFrom-Json 
$modJson.version = $qpmJson.info.version

# add the thing to the libs list because we don't need it as a mod file
$modJson.modFiles = $modlib
$modJson.libraryFiles = $libs
$modText = $modJson | ConvertTo-Json -Depth 50 -EscapeHandling EscapeNonAscii

Set-Content $mod $modText

# if the qmod exists, rename it to zip to update it, we'll rename it back later
if (Test-Path $qmod) 
{
    move-item -Force $qmod $zip
}
$msg = "Creating qmod for module " + $env:module_id + " With name " + $qmod
echo $msg
Compress-Archive -Path $fileList -DestinationPath $zip -Update

& move-item -Force $zip $qmod