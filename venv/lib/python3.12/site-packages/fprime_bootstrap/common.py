import sys
import shutil
import subprocess
from pathlib import Path
import logging

LOGGER = logging.getLogger("fprime_bootstrap")


def run_system_checks():
    """Runs system checks"""
    # Check Python version
    if sys.version_info < (3, 8):
        raise UnsupportedPythonVersion(
            "Python 3.8 or higher is required to use the F´ Python tooling suite. "
            "Please install Python 3.8 or higher and try again."
        )

    # Check if Git is installed and available - needed for cloning F´ as submodule
    if not shutil.which("git"):
        raise GitNotInstalled(
            "Git is not installed or in PATH. Please install Git and try again."
        )

    # Check if running on Windows
    if sys.platform == "win32":
        raise UnsupportedPlatform(
            "F´ does not currently support Windows. Please use WSL (https://learn.microsoft.com/en-us/windows/wsl/about), "
            "or a Linux or macOS system. If you are using WSL, please ensure you are running this script from WSL."
        )
    return 0


def setup_venv(project_path: Path, fprime_subpath: Path = Path("fprime")):
    """Sets up a new virtual environment"""
    venv_path = project_path / "fprime-venv"

    LOGGER.info(f"Creating virtual environment in {venv_path} ...")
    subprocess.run([Path(sys.executable), "-m", "venv", venv_path])

    # Find pip
    pip = None
    if (venv_path / "bin/pip").exists():
        pip = venv_path / "bin/pip"
    elif (venv_path / "bin/pip3").exists():
        pip = venv_path / "bin/pip3"
    else:
        raise FileNotFoundError("Could not find pip executable in venv.")

    LOGGER.info("Upgrading pip...")
    subprocess.run([pip, "install", "--upgrade", "pip"])

    LOGGER.info("Installing F´ dependencies...")
    subprocess.run(
        [
            pip,
            "install",
            "-Ur",
            project_path / fprime_subpath / "requirements.txt",
        ]
    )


def print_success_message(project_name: str):
    """Prints a success message"""
    print(
        f"""
################################################################

Congratulations! You have successfully created a new F´ project.

A git repository has been initialized and F´ has been added as a
submodule, you can now create your first commit.

Get started with your F´ project:

-- Remember to always activate the virtual environment --
cd {project_name}
. fprime-venv/bin/activate

-- Create a new component --
fprime-util new --component

-- Create a new deployment --
fprime-util new --deployment

################################################################
"""
    )


#################### Exceptions ####################


class BootstrapError(Exception):
    pass


class BootstrapProjectError(BootstrapError):
    pass


class UnsupportedPythonVersion(BootstrapProjectError):
    pass


class GitNotInstalled(BootstrapProjectError):
    pass


class GitCloneError(BootstrapProjectError):
    pass


class UnsupportedPlatform(BootstrapProjectError):
    pass


class InvalidProjectName(BootstrapProjectError):
    pass


class OutDirectoryError(BootstrapProjectError):
    pass
