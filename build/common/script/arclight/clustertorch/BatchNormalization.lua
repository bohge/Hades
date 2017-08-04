local BatchNormalization = nn.BatchNormalization

function BatchNormalization:subparameters()
  if self.running_mean and self.running_var then
    return {self.running_mean, self.running_var};
  else
    return
  end
end