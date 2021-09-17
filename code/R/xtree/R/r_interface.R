
convertColTypes = function(frame) {
  res = unlist(lapply(frame, class))
  res = lapply(res, FUN = function(x) {
    if (x == "numeric") {
      return("num")
    } else if (x == "factor") {
      return("categ")
    }
  })
  res = unlist(res)
  res = unname(res)
  return(res)
}

convertCategEncodings = function(data) {
  types = convertColTypes(data)
  categ_ix = which(types == "categ")
  encodings = lapply(categ_ix, FUN = function(j) {
    unique(as.double(data[ , j])) - 1
  })
  names(encodings) = categ_ix - 1
  return(encodings)
}

