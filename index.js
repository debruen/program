
const {program_data, program_update, program_buffer, program_quit} = require('./build/Release/emit_from_cpp.node')

class Program{

  constructor() {

  }

  data() {
    return new Promise((resolve) => {
      program_data((err, result) => {
        const json = JSON.parse(result)
        resolve(json)
      })
    })
  }

  update(data) {
    const string = JSON.stringify(data)
    return new Promise((resolve) => {
      program_update(string, (err, result) => {
        const json = JSON.parse(result)
        resolve(json)
      })
    })
  }

  buffer(data, image) {
    const string = JSON.stringify(data)
    return new Promise((resolve) => {
      program_buffer(string, image, (err, result) => {
        const json = JSON.parse(result)
        resolve(json)
      })
    })

  }

  quit() {
    return new Promise((resolve) => {
      program_quit((err, result) => {
        resolve(result)
      })
    })
  }

}

module.exports = Program;
