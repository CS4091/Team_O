# Beginning
$hasErrors = $false

$successColor = "Green"
$errorColor = "Red"
# $infoColor = "Cyan"

# Sort imports according to conventions specified in pyproject.toml
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
Write-Host "Sorting imports with isort...."
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

isort .

if ( $LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "isort found issues." -ForegroundColor $errorColor
    $hasErrors = $true
}
else {
    Write-Host ""
    Write-Host "isort completed successfully." -ForegroundColor $successColor
}
Write-Host ""

# Format code with Black, applying settings from pyproject.toml and displaying color output
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
Write-Host "Formatting code with Black...."
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

black --color .

if ( $LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "Black formatter found issues." -ForegroundColor $errorColor
    $hasErrors = $true
}
else {
    Write-Host ""
    Write-Host "Black completed successfully." -ForegroundColor $successColor
}
Write-Host ""

# Run type-checking with mypy using configurations from pyproject.toml
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
Write-Host "Running type checks with mypy...."
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

mypy .

if ( $LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "mypy found issues." -ForegroundColor $errorColor
    $hasErrors = $true
}
else {
    Write-Host ""
    Write-Host "mypy completed successfully." -ForegroundColor $successColor
}
Write-Host ""

# Lint code for style and PEP8 compliance with flake8
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
Write-Host "Linting code with flake8...."
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

flake8

if ( $LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "flake8 linter found issues." -ForegroundColor $errorColor
    $hasErrors = $true
}
else {
    Write-Host ""
    Write-Host "flake8 completed successfully." -ForegroundColor $successColor
}
Write-Host ""

# Test if there are valid docstrings for project.
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
Write-Host "Checking docstrings with pydocstyle...."
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

pydocstyle

if ( $LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "pydocstyle found issues." -ForegroundColor $errorColor
    $hasErrors = $true
}
else {
    Write-Host ""
    Write-Host "pydocstyle completed successfully." -ForegroundColor $successColor
}
Write-Host ""

# Final
if ( $hasErrors ) {
    Write-Host ""
    Write-Host "One or more tools have found errors in the code..." -ForegroundColor $errorColor
    Write-Host "Please fix the errors in the code before pushing to version control." -ForegroundColor $errorColor
}
else {
    Write-Host ""
    Write-Host "All tools have executed and no issues were found." -ForegroundColor $successColor
}