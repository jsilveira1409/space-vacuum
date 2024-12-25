"""
fprime_bootstrap.bootstrap_project:

Bootstraps a new project using cookiecuter

@author thomas-bc
"""

import configparser
import logging
import subprocess
from pathlib import Path
from fprime_bootstrap.common import (
    run_system_checks,
    setup_venv,
    print_success_message,
    OutDirectoryError,
    GitCloneError,
)

from typing import TYPE_CHECKING, Tuple

if TYPE_CHECKING:
    import argparse


LOGGER = logging.getLogger("fprime_bootstrap")


def clone_project(parsed_args: "argparse.Namespace"):
    """Creates a new F´ project"""

    # Runs system checks such as Python version, OS requirements etc...
    run_system_checks()

    target_dir = Path(parsed_args.path)

    try:
        project_path, fprime_path = clone_git_repo(
            target_dir, parsed_args.url, parsed_args.rename
        )
        if not parsed_args.no_venv:
            setup_venv(project_path, fprime_path)

        print_success_message(project_path)

    except (PermissionError, FileExistsError) as out_directory_error:
        raise OutDirectoryError(
            f"{out_directory_error}. Please select a different project name or remove the existing directory."
        )
    except FileNotFoundError as e:
        raise OutDirectoryError(
            f"{e}. Permission denied to write to the directory.",
        )
    return 0


def clone_git_repo(
    target_dir: Path, remote_url: str, new_name: str = None
) -> Tuple[Path, Path]:
    """Clone an F´ project using git. Uses new_name if provided for local project name
    Returns the path to the project and the path to the F´ submodule within the project
    """

    if new_name:
        target_name = new_name
    else:
        # Extract the repository name from the remote_url
        target_name = remote_url.rstrip("/").split("/")[-1]
        # Remove .git extension if present and patiently wait for Python3.9 for str.removesuffix()
        if target_name.endswith(".git"):
            target_name = target_name[:-4]

    # Add F´ as a submodule
    LOGGER.info(f"Cloning out F´ project in {target_dir} ...")
    run = subprocess.run(
        [
            "git",
            "clone",
            "--recurse-submodules",
            "--shallow-submodules",
            remote_url,
            target_name,
        ],
        cwd=target_dir,
    )

    if run.returncode != 0:
        raise GitCloneError(f"Failed to clone repository: {remote_url}")

    project_path = target_dir / target_name
    fprime_path = find_fprime_path(project_path)

    return project_path, fprime_path


def find_fprime_path(project_path: Path) -> Path:
    """Find the path to the F´ submodule within the project"""

    settings_ini_file = project_path / "settings.ini"

    if not settings_ini_file.exists():
        raise FileNotFoundError(
            "settings.ini not found in project - project not correctly formed"
        )

    settings_ini = configparser.ConfigParser()
    settings_ini.read(settings_ini_file)

    if "fprime" not in settings_ini:
        raise KeyError(
            "fprime section not found in settings.ini - project not correctly formed"
        )

    if "framework_path" not in settings_ini["fprime"]:
        raise KeyError(
            "framework_path not found in fprime section of settings.ini - project not correctly formed"
        )

    return settings_ini["fprime"]["framework_path"]
