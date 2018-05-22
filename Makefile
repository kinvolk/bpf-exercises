DOCKER_IMAGE=kinvolk/bpf-exercises
SUDO=$(shell docker info >/dev/null 2>&1 || echo "sudo -E")

default:
	@echo "Please choose a target"
	@exit 1

.PHONY: default

container:
	$(SUDO) docker build -t $(DOCKER_IMAGE) .

.PHONY: container

run-container:
	$(SUDO) docker run --rm --privileged -ti -v $(PWD):/bpf-exercises $(DOCKER_IMAGE) bash

.PHONY: run-container

exercise-01:
	$(SUDO) docker run --rm --privileged -ti -v $(PWD):/bpf-exercises $(DOCKER_IMAGE) \
		bash -c 'cd $@ && make'

.PHONY: exercise-01
