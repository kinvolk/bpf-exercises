DOCKER_IMAGE=kinvolk/bpf-exercises
SUDO=$(shell docker info >/dev/null 2>&1 || echo "sudo -E")

EXERCISES=$(wildcard exercise-*)
BUILD_TARGETS=$(patsubst %,build-%,$(EXERCISES))
RUN_TARGETS=$(patsubst %,run-%,$(EXERCISES))

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

$(BUILD_TARGETS):
	$(SUDO) docker run --rm --privileged -ti -v $(PWD):/bpf-exercises $(DOCKER_IMAGE) \
		bash -c 'cd $(@:build-%=%) && make build'

.PHONY: $(BUILD_TARGETS)

$(RUN_TARGETS):
	$(SUDO) docker run --rm --privileged -ti -v $(PWD):/bpf-exercises $(DOCKER_IMAGE) \
		bash -c 'cd $(@:run-%=%) && make run'

.PHONY: $(RUN_TARGETS)
