variable "hcloud_token" {
  type = "string"
}

# The keys need to be added manually to the Hetzner cloud account.
# The list contains the names of the keys, not the keys itself.
variable "hcloud_ssh_keys_names" {
  type = "list"
  default = []
}
