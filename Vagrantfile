Vagrant.configure("2") do |config|
  #config.vm.box = "chef/ubuntu-14.04"
  config.vm.box = "hashicorp/precise64"

  config.vm.provider "virtualbox" do |vb|
     vb.customize ["modifyvm", :id, "--memory", "4096", "--cpus", "4", "--cpuexecutioncap", "100"]
  end
end
