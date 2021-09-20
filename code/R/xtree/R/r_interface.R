
convertColTypes = function(frame) {
  frame = df
  res = unlist(lapply(frame, class))
  res = lapply(res, FUN = function(x) {
    if ((x == "numeric") || (x == "integer")) {
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

xtree = function(data, target, params) {
  coltypes = convertColTypes(data)
  categ_encodings = convertCategEncodings(data)
  
  tree = XTree$new(data, target, coltypes, categ_encodings, params)
  return(tree)
}



library(partykit)
as_party = function(tree, data) {
  
}
# party_interface = function(tree.summary, data) {
#   options(scipen = 99)
#   
#   features = colnames(data)
#   party.node.list = list()
#   depth = max(tree.summary$depth)
#   nodes = tree.summary[which(tree.summary$depth == depth), ]
#   nodes.id = as.numeric(as.character(nodes$id))
#   
#   party.nodes = lapply(nodes.id, FUN = function(node.id) {
#     
#     node.ame = nodes[nodes$id == node.id, "ame"]
#     node.sd = nodes[nodes$id == node.id, "ame.sd"]
#     node.n = nodes[nodes$id == node.id, "n.observations"]
#     node.frac = round(as.numeric(as.character(node.n)) / nrow(data) * 100, 2)
#     node.frac = paste0(node.frac, "%")
#     node.info = paste0("AME: ", node.ame, "\n", "SD: ", node.sd, "\n", "Frac.: ", node.frac)
#     party.node = partynode(id = node.id, info = node.info)
#     return(party.node)
#   })
#   
#   party.node.list = append(party.node.list, party.nodes)
#   depth = depth - 1
#   
#   while (depth > 0) {
#     
#     nodes = tree.summary[tree.summary$depth == depth, ]
#     parent.nodes = nodes[!is.na(nodes$split.feature), ]
#     leaf.nodes = nodes[is.na(nodes$split.feature), ]
#     
#     parent.party.nodes = lapply(1:nrow(parent.nodes), FUN = function(i) {
#       
#       node = parent.nodes[i, ]
#       node.id = as.numeric(as.character(node$id))
#       node.split.feature = node$split.feature
#       node.split.value = node$split.value
#       node.split.feature.index = which(features == node.split.feature)
#       
#       child.party.nodes = lapply(party.node.list, FUN = function(x) {
#         
#         x.splitstring = unlist(strsplit(as.character(x$id), ""))
#         x.pasted = paste0(x.splitstring[-length(x.splitstring)], collapse = "")
#         if (node.id == x.pasted) {
#           return(x)
#         } else {
#         }
#       })
#       
#       child.party.nodes = child.party.nodes[!unlist(lapply(child.party.nodes, is.null))]
#       # drop NULL values
#       
#       child.party.directions = lapply(child.party.nodes, FUN = function(x) {
#         
#         x = unlist(strsplit(as.character(x$id), split = ""))
#         if (x[length(x)] == "0") {
#           return("child.left")
#         } else {
#           return("child.right")
#         }
#       })
#       
#       names(child.party.nodes) = child.party.directions
#       child.party.node.left = child.party.nodes[["child.left"]]
#       child.party.node.right = child.party.nodes[["child.right"]]
#       
#       party.node = partynode(
#         node.id, split = partysplit(node.split.feature.index, node.split.value),
#         kids = list(child.party.node.left, child.party.node.right))
#       return(party.node)
#     })
#     
#     if (nrow(leaf.nodes) > 0) {
#       leaf.party.nodes = lapply(1:nrow(leaf.nodes), FUN = function(i) {
#         node = leaf.nodes[i, ]
#         node.id = as.numeric(as.character(node$id))
#         node.ame = nodes[nodes$id == node.id, "ame"]
#         node.sd = nodes[nodes$id == node.id, "ame.sd"]
#         node.n = nodes[nodes$id == node.id, "n.observations"]
#         node.frac = round(as.numeric(as.character(node.n)) / nrow(data) * 100, 2)
#         node.frac = paste0(node.frac, "%")
#         node.info = paste0("AME: ", node.ame, "\n", "SD: ", node.sd, "\n", "Frac.: ", node.frac)
#         party.node = partynode(id = node.id, info = node.info)
#         return(party.node)
#       })
#     } else {}
#     
#     party.node.list = append(party.node.list, parent.party.nodes)
#     party.node.list = append(party.node.list, leaf.party.nodes)
#     
#     depth = depth - 1
#   }
#   
#   tree.index = which(unlist(lapply(party.node.list, FUN = function(x) {
#     x$id == 1
#   })))
#   
#   party.tree = party.node.list[[tree.index]]
#   party.tree = party(party.tree, data)
#   # fitted = data.frame(
#   #   "(fitted)" = fitted_node(party.tree,
#   #                            data = data),
#   #   "(response)" = data$marginal.effect),
#   # terms = terms(marginal.effect ~ ., data = data),)
#   return(party.tree)
# }

