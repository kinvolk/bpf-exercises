resource "hcloud_server" "bpf-playground" {
  lifecycle {
    ignore_changes = ["ssh_keys"]
  }
  count = 1
  name = "bpf-playground-${count.index}"
  image = "ubuntu-18.04"
  server_type = "cx11"
  ssh_keys = "${var.hcloud_ssh_keys_names}"
  datacenter = "fsn1-dc8"
  user_data = "${file("user-data.bash")}"
}
