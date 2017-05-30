
import proc from './proc'

export default {
  template: `<transition name="slide-fade">
                <div class="content">
                  <div class="processes">
                    <table class="stable">
                      <!--<caption></caption>-->
                      <thead>
                        <tr>
                          <th scope="col">PID</th>
                          <th scope="col">Process Name</th>
                          <th scope="col">Status</th>
                        </tr>
                      </thead>
                      <tbody>
                        <tr v-for="p in processes">
                          <td>{{ p.pid }}</th>
                          <td>{{ p.comm }}</td>
                          <td>{{ p.state }}</td>
                        </tr>
                        <tr>
                          <td>Andrew Chalkley</th>
                          <td>andrew@example.com</td>
                          <td>Front-End Developer</td>
                        </tr>
                        <tr>
                          <td>Dave McFarland</th>
                          <td>dave@example.com</td>
                          <td>Front-End Developer</td>
                        </tr>
                      </tbody>
                    </table>
                  </div>
                </div>
              </transition>`,
  data() {
    return({
      processes: proc()
    })
  },
  created() {
  },
  components: {
    
  }
}