"""
fprime_bootstrap.__main__:

Main entry point for fprime-bootstrap

@author thomas-bc
"""

import sys
import os

import logging
import argparse

from fprime_bootstrap.bootstrap_project import bootstrap_project
from fprime_bootstrap.clone_project import clone_project
from fprime_bootstrap.common import BootstrapError

logging.basicConfig(
    format="[%(levelname)s] %(message)s",
    level=logging.INFO,
)
LOGGER = logging.getLogger("fprime_bootstrap")


def main():
    """Run wrapper, to point a console_script at"""

    parser = argparse.ArgumentParser(description="F Prime bootstrapping tool")
    subparsers = parser.add_subparsers(title="subcommands", dest="command")
    project_parser = subparsers.add_parser("project", help="Create a new F´ project")
    project_parser.add_argument(
        "--path",
        type=str,
        help="Path to create the project in (default: current directory)",
        default=os.getcwd(),
    )
    project_parser.add_argument(
        "--no-venv",
        action="store_true",
        help="Do not create a virtual environment in the project",
        default=False,
    )
    project_parser.add_argument(
        "--tag",
        type=str,
        help="Version of F´ to checkout (default: latest release)",
    )

    clone_parser = subparsers.add_parser(
        "clone", help="Clone an existing remote F´ project"
    )
    clone_parser.add_argument(
        dest="url",
        type=str,
        help="URL of the remote repository to clone",
    )
    clone_parser.add_argument(
        "--path",
        type=str,
        help="Path to create the project in (default: current directory)",
        default=os.getcwd(),
    )
    clone_parser.add_argument(
        "--no-venv",
        action="store_true",
        help="Do not create a virtual environment in the project",
        default=False,
    )
    clone_parser.add_argument(
        "--rename",
        type=str,
        help="Name of the local project directory (default: remote repository name)",
    )
    args = parser.parse_args()

    try:
        if args.command == "project":
            return bootstrap_project(args)

        if args.command == "clone":
            return clone_project(args)

    except BootstrapError as e:
        LOGGER.error(e)
        return 1

    LOGGER.error("No sub-command supplied")
    parser.print_help()
    sys.exit(1)


if __name__ == "__main__":
    sys.exit(main())
